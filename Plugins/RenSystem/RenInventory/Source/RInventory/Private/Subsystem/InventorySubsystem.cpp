// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/InventorySubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Definition/FilterContext.h"
#include "Definition/AssetFilterProperty.h"
#include "Definition/InventoryFilterProperty.h"
#include "Definition/InventoryItem.h"
#include "Definition/InventoryPrimaryAsset.h"
#include "Definition/InventoryQueryType.h"
#include "Definition/InventorySortType.h"
#include "Definition/InventoryStack.h"
#include "Asset/InventoryAsset.h"
#include "Filter/FilterCriterion.h"
#include "Interface/StorageProviderInterface.h"
#include "LatentDelegates.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.h"
#include "Storage/InventoryStorage.h"



bool UInventorySubsystem::AddItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Quantity)
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);
	
	return AddItem_Internal(InventoryId, AssetId, Quantity, Stack);
}

bool UInventorySubsystem::AddItems(const FGuid& InventoryId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	UInventoryStorage* Storage = GetStorage(InventoryId);

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);
		AddItem_Internal(InventoryId, AssetId, Quantity, Stack);
	}

	return true;
}



bool UInventorySubsystem::RemoveItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Quantity)
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);

	return RemoveItem_Internal(InventoryId, AssetId, Quantity, Stack);
}

bool UInventorySubsystem::RemoveItems(const FGuid& InventoryId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	if (!ContainItems(InventoryId, Items, Multiplier))
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	bool bResult = true;

	UInventoryStorage* Storage = GetStorage(InventoryId);

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);
		bool bRemoved = RemoveItem_Internal(InventoryId, AssetId, Quantity, Stack);
		if (!bRemoved)
		{
			bResult = false;
			break;
		}
	}

	return true;
}

bool UInventorySubsystem::RemoveAnyItems(const FGuid& InInventoryId, const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity)
{
	FPrimaryAssetId SelectedAssetId;
	int SelectedQuantity = 0;

	if (!ContainAnyItems(InInventoryId, InItems, InMultiplier, SelectedAssetId, SelectedQuantity))
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	bool bRemoved = RemoveItem(InInventoryId, SelectedAssetId, SelectedQuantity);
	if (!bRemoved)
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	OutAssetId = SelectedAssetId;
	OutQuantity = SelectedQuantity;
	return true;
}

bool UInventorySubsystem::RemoveItemById(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity)
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);
	bool bRemoved = RemoveItemById_Internal(InventoryId, AssetId, ItemId, Quantity, Stack);

	return bRemoved;
}



bool UInventorySubsystem::UpdateItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TFunctionRef<void(FInventoryItem*)> Callback)
{
	FInventoryItem* Item = GetMutableItemByIndex(InventoryId, AssetId, 0);
	if (!Item)
	{
		return false;
	}

	Callback(Item);

	OnItemUpdated.Broadcast(InventoryId, AssetId, Item->ItemId);
	OnInventoryRefreshed.Broadcast(InventoryId);

	return true;
}

bool UInventorySubsystem::UpdateItemById(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, TFunctionRef<void(FInventoryItem*)> Callback)
{
	FInventoryItem* Item = GetMutableItemById(InventoryId, AssetId, ItemId);
	if (!Item)
	{
		return false;
	}

	Callback(Item);

	OnItemUpdated.Broadcast(InventoryId, AssetId, Item->ItemId);
	OnInventoryRefreshed.Broadcast(InventoryId);

	return true;
}


bool UInventorySubsystem::ContainItems(const FGuid& InventoryId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const
{
	bool bResult = true;
	UInventoryStorage* Storage = GetStorage(InventoryId);

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int RequiredQuantity = Kv.Value * Multiplier;

		if (!InventoryPrimaryAsset::IsValid(AssetId) || RequiredQuantity <= 0)
		{
			bResult = false;
			break;
		}

		const FInventoryStack* Stack = GetStack(AssetId, Storage);
		if (!Stack)
		{
			bResult = false;
			break;
		}

		const TArray<FInventoryItem>& ItemList = Stack->ItemList;
		bool bStackable = Stack->bStackable;
		int ItemQuantity = 0;
		if (bStackable)
		{
			if (!ItemList.IsValidIndex(0))
			{
				bResult = false;
				break;
			}

			ItemQuantity = ItemList[0].Quantity;
		}
		else
		{
			ItemQuantity = ItemList.Num();
		}

		if (ItemQuantity < RequiredQuantity)
		{
			bResult = false;
			break;
		}
	}

	return bResult;
}

bool UInventorySubsystem::ContainAnyItems(const FGuid& InInventoryId, const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const
{
	bool bResult = false;
	UInventoryStorage* Storage = GetStorage(InInventoryId);

	for (const TPair<FPrimaryAssetId, int>& Kv : InItems)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int RequiredQuantity = Kv.Value * InMultiplier;

		if (!InventoryPrimaryAsset::IsValid(AssetId) || RequiredQuantity <= 0)
		{
			continue;
		}

		const FInventoryStack* Stack = GetStack(AssetId, Storage);
		if (!Stack)
		{
			continue;
		}

		const TArray<FInventoryItem>& ItemList = Stack->ItemList;
		bool bStackable = Stack->bStackable;
		int ItemQuantity = 0;
		if (bStackable)
		{
			if (!ItemList.IsValidIndex(0))
			{
				continue;
			}

			const FInventoryItem& Record = ItemList[0];
			ItemQuantity = Record.Quantity;
		}
		else
		{
			ItemQuantity = ItemList.Num();
		}

		if (ItemQuantity >= RequiredQuantity)
		{
			OutAssetId = AssetId;
			OutQuantity = RequiredQuantity;

			bResult = true;
			break;
		}
	}

	return bResult;
}



int UInventorySubsystem::GetTotalQuantity(const FGuid& InventoryId, const FPrimaryAssetId& AssetId) const
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	return GetItemQuantity_Internal(InventoryId, AssetId, Storage);
}

const FInventoryItem* UInventorySubsystem::GetItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId) const
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	const FInventoryStack* Stack = GetStack(AssetId, Storage);
	if (!Stack)
	{
		return nullptr;
	}

	const TArray<FInventoryItem>& RecordList = Stack->ItemList;
	if (RecordList.Num() <= 0)
	{
		return nullptr;
	}

	return &RecordList[0];
}

const FInventoryItem* UInventorySubsystem::GetItemById(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId) const
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	const FInventoryStack* Stack = GetStack(AssetId, Storage);
	if (!Stack)
	{
		return nullptr;
	}

	const TArray<FInventoryItem>& RecordList = Stack->ItemList;
	const FInventoryItem* Record = RecordList.FindByPredicate([ItemId](const FInventoryItem& Record) { return Record.ItemId == ItemId; });

	return Record;
}



void UInventorySubsystem::QueryItems(const FGuid& InventoryId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback)
{
	if (QueryRule.QuerySource == EInventoryQuerySource::Glossary)
	{
		HandleGlossaryItems(InventoryId, FilterCriterion, QueryRule, MoveTemp(Callback));
	}
	else
	{
		HandleInventoryItems(InventoryId, FilterCriterion, QueryRule, MoveTemp(Callback));
	}
}

FInventoryItem* UInventorySubsystem::GetMutableItemByIndex(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Index) const
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	FInventoryStack* Stack = GetStack(AssetId, Storage);
	if (!Stack)
	{
		return nullptr;
	}

	TArray<FInventoryItem>& ItemList = Stack->ItemList;
	if (!ItemList.IsValidIndex(Index))
	{
		return nullptr;
	}

	return &ItemList[Index];
}

FInventoryItem* UInventorySubsystem::GetMutableItemById(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId) const
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	FInventoryStack* Stack = GetStack(AssetId, Storage);
	if (!Stack)
	{
		return nullptr;
	}

	TArray<FInventoryItem>& ItemList = Stack->ItemList;
	FInventoryItem* Item = ItemList.FindByPredicate([ItemId](const FInventoryItem& Record) { return Record.ItemId == ItemId; });

	return Item;
}

void UInventorySubsystem::HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const
{
	const ESortDirection& SortDirection = QueryRule.SortDirection;
	const EInventorySortType& SortType = QueryRule.SortType;

	switch (SortType)
	{
	case EInventorySortType::None:
		LOG_WARNING(LogInventory, TEXT("No sort implemented"));
		break;
	case EInventorySortType::Alphabetical:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				return (SortDirection == ESortDirection::Ascending) ?
					A.ItemName.ToString() < B.ItemName.ToString() :
					A.ItemName.ToString() > B.ItemName.ToString();
			}
		);
		break;
	case EInventorySortType::Quantity:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				return (SortDirection == ESortDirection::Ascending) ?
					A.Quantity < B.Quantity :
					A.Quantity > B.Quantity;
			}
		);
		break;
	case EInventorySortType::Level:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				if (!A.Item || !B.Item) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Item->Ascension.Level < B.Item->Ascension.Level :
					A.Item->Ascension.Level > B.Item->Ascension.Level;
			}
		);
		break;
	case EInventorySortType::Rank:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				if (!A.Item || !B.Item) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Item->Ascension.Rank < B.Item->Ascension.Rank :
					A.Item->Ascension.Rank > B.Item->Ascension.Rank;
			}
		);
		break;
	default:
		LOG_WARNING(LogInventory, TEXT("No sort implemented"));
		break;
	}
}

void UInventorySubsystem::HandleGlossaryItems(const FGuid& InventoryId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	if (!IsValid(Storage))
	{
		return;
	}

	TArray<FPrimaryAssetId> AssetIds;
	AssetManager->GetPrimaryAssetIdList(InventoryPrimaryAsset::GetAssetType(), AssetIds);

	TArray<FInventorySortEntry> SortedItems;
	SortedItems.Reserve(AssetIds.Num());

	for (const FPrimaryAssetId& AssetId : AssetIds)
	{
		FAssetData AssetData;
		if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
		{
			continue;
		}

		FText DisplayName = FText::GetEmpty();
		FName ItemType = NAME_None;
		FName ItemRarity = NAME_None;

		InventoryPrimaryAsset::GetDisplayName(AssetData, DisplayName);
		InventoryPrimaryAsset::GetType(AssetData, ItemType);
		InventoryPrimaryAsset::GetRarity(AssetData, ItemRarity);

		if (IsValid(FilterCriterion))
		{
			FFilterContext Context;
			Context.SetValue(AssetFilterProperty::AssetId, AssetId);
			Context.SetValue(InventoryFilterProperty::ItemType, ItemType);
			Context.SetValue(InventoryFilterProperty::ItemRarity, ItemRarity);

			if (!FilterCriterion->Evaluate(Context))
			{
				continue;
			}
		}

		int ItemQuantity = GetItemQuantity_Internal(InventoryId, AssetId, Storage);

		SortedItems.Emplace(AssetId, DisplayName, ItemQuantity);
	}

	HandleItemSorting(SortedItems, QueryRule);

	for (const FInventorySortEntry& Item : SortedItems)
	{
		Callback(Item);
	}
}

void UInventorySubsystem::HandleInventoryItems(const FGuid& InventoryId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const
{
	UInventoryStorage* Storage = GetStorage(InventoryId);
	if (!IsValid(Storage))
	{
		return;
	}

	TArray<FInventorySortEntry> SortedItems;
	const TMap<FPrimaryAssetId, FInventoryStack>& Stacks = Storage->InventoryStacks;
	for (const TPair<FPrimaryAssetId, FInventoryStack>& Kv : Stacks)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		const FInventoryStack* Stack = &Kv.Value;
		if (!InventoryPrimaryAsset::IsValid(AssetId) || !Stack)
		{
			continue;
		}

		FAssetData AssetData;
		if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
		{
			continue;
		}

		FText DisplayName = FText::GetEmpty();
		FName ItemType = TEXT_EMPTY;
		FName ItemRarity = TEXT_EMPTY;

		bool bNameValid = InventoryPrimaryAsset::GetDisplayName(AssetData, DisplayName);
		bool bTypeValid = InventoryPrimaryAsset::GetType(AssetData, ItemType);
		bool bRarityValid = InventoryPrimaryAsset::GetRarity(AssetData, ItemRarity);
		if (!bNameValid || !bTypeValid || !bRarityValid)
		{
			continue;
		}

		const TArray<FInventoryItem>& ItemList = Stack->ItemList;
		for (const FInventoryItem& Item : ItemList)
		{
			int ItemQuantity = Item.Quantity;
			if (IsValid(FilterCriterion))
			{
				FFilterContext Context;
				Context.SetValue(AssetFilterProperty::AssetId, AssetId);
				Context.SetValue(InventoryFilterProperty::ItemType, ItemType);
				Context.SetValue(InventoryFilterProperty::ItemRarity, ItemRarity);
				Context.SetValue(InventoryFilterProperty::ItemId, FName(*Item.ItemId.ToString()));
				Context.SetValue(InventoryFilterProperty::ItemQuantity, ItemQuantity);

				if (!FilterCriterion->Evaluate(Context))
				{
					continue;
				}
			}

			SortedItems.Emplace(AssetId, DisplayName, ItemQuantity, &Item);
		}
	}

	HandleItemSorting(SortedItems, QueryRule);

	for (const FInventorySortEntry& Item : SortedItems)
	{
		Callback(Item);
	}
}



UInventoryStorage* UInventorySubsystem::GetStorage(const FGuid& InventoryId) const
{
	IStorageProviderInterface* StorageProviderPtr = StorageProvider.Get();
	if (!StorageProviderPtr)
	{
		return nullptr;
	}
	return StorageProviderPtr->GetStorage<UInventoryStorage>(InventoryId);
}

TMap<FPrimaryAssetId, FInventoryStack>* UInventorySubsystem::GetStackCollection(UInventoryStorage* Storage) const
{
	if (!IsValid(Storage))
	{
		return nullptr;
	}
	return &Storage->InventoryStacks;
}

FInventoryStack* UInventorySubsystem::GetStack(const FPrimaryAssetId& AssetId, UInventoryStorage* Storage) const
{
	TMap<FPrimaryAssetId, FInventoryStack>* StackCollection = GetStackCollection(Storage);
	if (!StackCollection)
	{
		return nullptr;
	}
	return StackCollection->Find(AssetId);
}

FInventoryStack* UInventorySubsystem::FindOrAddStack(const FPrimaryAssetId& AssetId, UInventoryStorage* Storage)
{
	TMap<FPrimaryAssetId, FInventoryStack>* StackCollection = GetStackCollection(Storage);
	if (!StackCollection)
	{
		return nullptr;
	}

	FInventoryStack* Stack = StackCollection->Find(AssetId);
	if (Stack)
	{
		return Stack;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		return nullptr;
	}

	bool bStackable = false;
	bool bPersistWhenEmpty = false;

	bool bFoundStackable = InventoryPrimaryAsset::GetStackable(AssetData, bStackable);
	bool bFoundPersist = InventoryPrimaryAsset::GetPersistWhenEmpty(AssetData, bPersistWhenEmpty);
	if (!bFoundStackable || !bFoundPersist)
	{
		return nullptr;
	}

	FGuid StackId = FGuid::NewGuid();
	FInventoryStack& NewStack = StackCollection->Add(AssetId, { StackId, bStackable, bPersistWhenEmpty });

	return &NewStack;
}



int UInventorySubsystem::GetItemQuantity_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, UInventoryStorage* Storage) const
{
	const FInventoryStack* Stack = GetStack(AssetId, Storage);
	if (!Stack)
	{
		return 0;
	}

	const TArray<FInventoryItem>& ItemList = Stack->ItemList;
	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (!ItemList.IsValidIndex(0))
		{
			return 0;
		}
		return ItemList[0].Quantity;
	}

	return ItemList.Num();
}



bool UInventorySubsystem::AddItem_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !Stack)
	{
		LOG_ERROR(LogInventory, TEXT("AssetId, Stack is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TArray<FInventoryItem>& ItemList = Stack->ItemList;
	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (ItemList.IsValidIndex(0))
		{
			FInventoryItem& ItemData = ItemList[0];
			AddItem_InternalUpdate(InventoryId, AssetId, ItemData, Quantity);
			return true;
		}
		else
		{
			AddItem_InternalCreate(InventoryId, AssetId, ItemList, Quantity);
			return true;
		}
	}

	for (int i = 0; i < Quantity; i++)
	{
		AddItem_InternalCreate(InventoryId, AssetId, ItemList, 1);
	}

	return true;
}

void UInventorySubsystem::AddItem_InternalCreate(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity)
{
	FInventoryItem Item;
	Item.ItemId = FGuid::NewGuid();
	Item.Quantity = Quantity;
	ItemList.Add(Item);

	OnItemAdded.Broadcast(InventoryId, AssetId, Item.ItemId);
	OnInventoryRefreshed.Broadcast(InventoryId);

	LOG_INFO(LogInventory, TEXT("New item added"));
}

void UInventorySubsystem::AddItem_InternalUpdate(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, FInventoryItem& Item, int Quantity)
{
	Item.Quantity += Quantity;

	OnItemAdded.Broadcast(InventoryId, AssetId, Item.ItemId);
	OnInventoryRefreshed.Broadcast(InventoryId);

	LOG_INFO(LogInventory, TEXT("Item updated"));
}



bool UInventorySubsystem::RemoveItem_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !Stack)
	{
		LOG_ERROR(LogInventory, TEXT("AssetId, Stack is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TArray<FInventoryItem>& ItemList = Stack->ItemList;
	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (ItemList.IsValidIndex(0))
		{
			bool bPersist = Stack->bPersistWhenEmpty;

			FInventoryItem& Record = ItemList[0];
			RemoveItem_InternalUpdate(InventoryId, AssetId, ItemList, bPersist, &Record, Quantity);

			return true;
		}

		LOG_ERROR(LogInventory, TEXT("Not item to remove"));
		return false;
	}

	bool bRemoved = RemoveItem_InternalStack(InventoryId, AssetId, ItemList, Quantity);
	return bRemoved;
}

bool UInventorySubsystem::RemoveItemById_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity, FInventoryStack* Stack)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || !ItemId.IsValid() || Quantity <= 0 || !Stack)
	{
		LOG_ERROR(LogInventory, TEXT("AssetId, ItemId, Stack is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TArray<FInventoryItem>& ItemList = Stack->ItemList;
	FInventoryItem* Item = ItemList.FindByPredicate([ItemId](const FInventoryItem& Item) { return Item.ItemId == ItemId; });
	if (!Item)
	{
		LOG_ERROR(LogInventory, TEXT("Item not found for ItemId"));
		return false;
	}

	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		bool bPersistWhenEmpty = Stack->bPersistWhenEmpty;
		RemoveItem_InternalUpdate(InventoryId, AssetId, ItemList, bPersistWhenEmpty, Item, Quantity);
		return true;
	}

	bool bRemoved = RemoveItem_InternalRemove(InventoryId, AssetId, ItemList, Item);
	return bRemoved;
}

bool UInventorySubsystem::RemoveItem_InternalStack(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity)
{
	int StackCount = ItemList.Num();
	if (Quantity > StackCount)
	{
		return false;
	}

	for (int i = 0; i < Quantity; i++)
	{
		FInventoryItem Item = ItemList.Pop();

		OnItemRemoved.Broadcast(InventoryId, AssetId, Item.ItemId);
		OnInventoryRefreshed.Broadcast(InventoryId);
	}

	return true;
}

bool UInventorySubsystem::RemoveItem_InternalRemove(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, FInventoryItem* Item)
{
	FInventoryItem TempItem = *Item;

	int Count = ItemList.Remove(TempItem);
	if (Count == 0)
	{
		return false;
	}

	OnItemRemoved.Broadcast(InventoryId, AssetId, TempItem.ItemId);
	OnInventoryRefreshed.Broadcast(InventoryId);

	return true;
}

void UInventorySubsystem::RemoveItem_InternalUpdate(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, bool bPersistWhenEmpty, FInventoryItem* Item, int Quantity)
{
	FGuid ItemId = Item->ItemId;
	int ItemQuantity = Item->Quantity;

	int NewQuantity = ItemQuantity - Quantity;
	if (NewQuantity > 0)
	{
		Item->Quantity = NewQuantity;

		OnItemRemoved.Broadcast(InventoryId, AssetId, ItemId);
		OnInventoryRefreshed.Broadcast(InventoryId);

		return;
	}

	if (bPersistWhenEmpty)
	{
		Item->Quantity = 0;

		OnItemRemoved.Broadcast(InventoryId, AssetId, ItemId);
		OnInventoryRefreshed.Broadcast(InventoryId);

		return;
	}

	RemoveItem_InternalRemove(InventoryId, AssetId, ItemList, Item);
}



void UInventorySubsystem::OnPreGameInitialized()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	IStorageProviderInterface* StorageProviderPtr = IStorageProviderInterface::Get(GetGameInstance());
	if (!StorageProviderPtr)
	{
		LOG_ERROR(LogInventory, TEXT("Storage subsystem not found"));
		return;
	}

	FStorageHandle Handle;
	Handle.StorageClass = UInventorySubsystem::GetStorageClass();
	Handle.StorageId = UInventorySubsystem::GetStorageId();
	Handle.Url = UInventorySubsystem::GetStorageUrl();

	StorageProviderPtr->LoadStorage(MoveTemp(Handle));

	StorageProvider = TWeakInterfacePtr<IStorageProviderInterface>(StorageProviderPtr);
}

bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventory, TEXT("InventorySubsystem initialized"));

	FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UInventorySubsystem::OnPreGameInitialized);
}

void UInventorySubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	LOG_WARNING(LogInventory, TEXT("InventorySubsystem deinitialized"));
	Super::Deinitialize();
}



UInventorySubsystem* UInventorySubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UInventorySubsystem* UInventorySubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UInventorySubsystem>();
}

FGuid UInventorySubsystem::GetStorageId()
{
	return FGuid(TEXT("24D61E46-5B41-49A4-BDFB-B1E39BF54665"));
}

FString UInventorySubsystem::GetStorageUrl()
{
	return TEXT("/api/get/inventory");
}

TSubclassOf<UStorage> UInventorySubsystem::GetStorageClass()
{
	return UInventoryStorage::StaticClass();
}


FPrimaryAssetType UInventorySubsystem::GetHandledAssetType() const
{
	return UInventoryAsset::GetPrimaryAssetType();
}

FGuid UInventorySubsystem::GetDefaultSlotId() const
{
	return UInventorySubsystem::GetStorageId();
}

