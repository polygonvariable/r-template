// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventorySubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RAssetManager.h"

#include "RCoreDelegate/Public/LatentDelegates.h"

#include "RCoreFilter/Public/FilterContext.h"
#include "RCoreFilter/Public/FilterCriterion.h"

#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryContainer.h"
#include "RCoreInventory/Public/InventoryItemType.h"
#include "RCoreInventory/Public/InventoryProviderInterface.h"
#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreStorage/Public/StorageUtils.h"

#include "InventoryDefinition.h"
#include "InventoryPrimaryAsset.h"

#include "InventoryStorageSubsystem.h"
#include "InventoryStorage.h"



bool UInventorySubsystem::AddItem(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	UInventoryStorage* Storage = GetStorage(ContainerId);
	FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);

	return AddItem_Internal(ContainerId, AssetId, Quantity, Stack);
}

bool UInventorySubsystem::AddItems(const FGuid& ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	UInventoryStorage* Storage = GetStorage(ContainerId);

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);
		AddItem_Internal(ContainerId, AssetId, Quantity, Stack);
	}

	return true;
}



bool UInventorySubsystem::RemoveItem(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	UInventoryStorage* Storage = GetStorage(ContainerId);
	FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);

	return RemoveItem_Internal(ContainerId, AssetId, Quantity, Stack);
}

bool UInventorySubsystem::RemoveItems(const FGuid& ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	if (!ContainItems(ContainerId, Items, Multiplier))
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	bool bResult = true;

	UInventoryStorage* Storage = GetStorage(ContainerId);

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);
		bool bRemoved = RemoveItem_Internal(ContainerId, AssetId, Quantity, Stack);
		if (!bRemoved)
		{
			bResult = false;
			break;
		}
	}

	return true;
}

bool UInventorySubsystem::RemoveAnyItems(const FGuid& InContainerId, const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity)
{
	FPrimaryAssetId SelectedAssetId;
	int SelectedQuantity = 0;

	if (!ContainAnyItems(InContainerId, InItems, InMultiplier, SelectedAssetId, SelectedQuantity))
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	bool bRemoved = RemoveItem(InContainerId, SelectedAssetId, SelectedQuantity);
	if (!bRemoved)
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	OutAssetId = SelectedAssetId;
	OutQuantity = SelectedQuantity;
	return true;
}

bool UInventorySubsystem::RemoveItemById(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity)
{
	UInventoryStorage* Storage = GetStorage(ContainerId);
	FInventoryStack* Stack = FindOrAddStack(AssetId, Storage);
	bool bRemoved = RemoveItemById_Internal(ContainerId, AssetId, ItemId, Quantity, Stack);

	return bRemoved;
}



bool UInventorySubsystem::ContainItems(const FGuid& ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const
{
	bool bResult = true;
	UInventoryStorage* Storage = GetStorage(ContainerId);

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

bool UInventorySubsystem::ContainAnyItems(const FGuid& InContainerId, const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const
{
	bool bResult = false;
	UInventoryStorage* Storage = GetStorage(InContainerId);

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



int UInventorySubsystem::GetTotalQuantity(const FGuid& ContainerId, const FPrimaryAssetId& AssetId) const
{
	UInventoryStorage* Storage = GetStorage(ContainerId);
	return GetItemQuantity_Internal(ContainerId, AssetId, Storage);
}

const FInventoryItem* UInventorySubsystem::GetItemById(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, const FGuid& ItemId) const
{
	UInventoryStorage* Storage = GetStorage(ContainerId);
	const FInventoryStack* Stack = GetStack(AssetId, Storage);
	if (!Stack)
	{
		return nullptr;
	}

	const TArray<FInventoryItem>& RecordList = Stack->ItemList;
	const FInventoryItem* Record = RecordList.FindByPredicate([ItemId](const FInventoryItem& Record) { return Record.ItemId == ItemId; });

	return Record;
}



void UInventorySubsystem::QueryItems(const FGuid& ContainerId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback)
{
	if (QueryRule.QuerySource == EInventoryQuerySource::Glossary)
	{
		HandleGlossaryItems(ContainerId, FilterCriterion, QueryRule, MoveTemp(Callback));
	}
	else
	{
		HandleInventoryItems(ContainerId, FilterCriterion, QueryRule, MoveTemp(Callback));
	}
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

void UInventorySubsystem::HandleGlossaryItems(const FGuid& ContainerId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const
{
	UInventoryStorage* Storage = GetStorage(ContainerId);
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
			Context.SetValue(InventoryFilterProperty::AssetId, AssetId);
			Context.SetValue(InventoryFilterProperty::AssetType, ItemType);
			Context.SetValue(InventoryFilterProperty::AssetRarity, ItemRarity);

			if (!FilterCriterion->Evaluate(Context))
			{
				continue;
			}
		}

		int ItemQuantity = GetItemQuantity_Internal(ContainerId, AssetId, Storage);

		SortedItems.Emplace(AssetId, DisplayName, ItemQuantity);
	}

	HandleItemSorting(SortedItems, QueryRule);

	for (const FInventorySortEntry& Item : SortedItems)
	{
		Callback(Item);
	}
}

void UInventorySubsystem::HandleInventoryItems(const FGuid& ContainerId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const
{
	UInventoryStorage* Storage = GetStorage(ContainerId);
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
				Context.SetValue(InventoryFilterProperty::AssetId, AssetId);
				Context.SetValue(InventoryFilterProperty::AssetType, ItemType);
				Context.SetValue(InventoryFilterProperty::AssetRarity, ItemRarity);
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



UInventoryStorage* UInventorySubsystem::GetStorage(const FGuid& ContainerId) const
{
	if (!IsValid(InventoryStorage))
	{
		return nullptr;
	}
	return InventoryStorage->GetStorage<UInventoryStorage>(ContainerId);
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



int UInventorySubsystem::GetItemQuantity_Internal(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, UInventoryStorage* Storage) const
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



bool UInventorySubsystem::AddItem_Internal(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
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
			AddItem_InternalUpdate(ContainerId, AssetId, ItemData, Quantity);
			return true;
		}
		else
		{
			AddItem_InternalCreate(ContainerId, AssetId, ItemList, Quantity);
			return true;
		}
	}

	for (int i = 0; i < Quantity; i++)
	{
		AddItem_InternalCreate(ContainerId, AssetId, ItemList, 1);
	}

	return true;
}

void UInventorySubsystem::AddItem_InternalCreate(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity)
{
	FInventoryItem Item;
	Item.ItemId = FGuid::NewGuid();
	Item.Quantity = Quantity;
	ItemList.Add(Item);

	OnItemAdded.Broadcast(ContainerId, AssetId, Item.ItemId);
	LOG_INFO(LogInventory, TEXT("New item added"));
}

void UInventorySubsystem::AddItem_InternalUpdate(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, FInventoryItem& Item, int Quantity)
{
	Item.Quantity += Quantity;

	OnItemAdded.Broadcast(ContainerId, AssetId, Item.ItemId);
	LOG_INFO(LogInventory, TEXT("Item updated"));
}



bool UInventorySubsystem::RemoveItem_Internal(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
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
			RemoveItem_InternalUpdate(ContainerId, AssetId, ItemList, bPersist, &Record, Quantity);

			return true;
		}

		LOG_ERROR(LogInventory, TEXT("Not item to remove"));
		return false;
	}

	bool bRemoved = RemoveItem_InternalStack(ContainerId, AssetId, ItemList, Quantity);
	return bRemoved;
}

bool UInventorySubsystem::RemoveItemById_Internal(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity, FInventoryStack* Stack)
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
		RemoveItem_InternalUpdate(ContainerId, AssetId, ItemList, bPersistWhenEmpty, Item, Quantity);
		return true;
	}

	bool bRemoved = RemoveItem_InternalRemove(ContainerId, AssetId, ItemList, Item);
	return bRemoved;
}

bool UInventorySubsystem::RemoveItem_InternalStack(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity)
{
	int StackCount = ItemList.Num();
	if (Quantity > StackCount)
	{
		return false;
	}

	for (int i = 0; i < Quantity; i++)
	{
		FInventoryItem Item = ItemList.Pop();
		OnItemRemoved.Broadcast(ContainerId, AssetId, Item.ItemId);
	}

	return true;
}

bool UInventorySubsystem::RemoveItem_InternalRemove(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, FInventoryItem* Item)
{
	FInventoryItem TempItem = *Item;

	int Count = ItemList.Remove(TempItem);
	if (Count == 0)
	{
		return false;
	}

	OnItemRemoved.Broadcast(ContainerId, AssetId, TempItem.ItemId);
	return true;
}

void UInventorySubsystem::RemoveItem_InternalUpdate(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, bool bPersistWhenEmpty, FInventoryItem* Item, int Quantity)
{
	FGuid ItemId = Item->ItemId;
	int ItemQuantity = Item->Quantity;

	int NewQuantity = ItemQuantity - Quantity;
	if (NewQuantity > 0)
	{
		Item->Quantity = NewQuantity;
		OnItemRemoved.Broadcast(ContainerId, AssetId, ItemId);
		return;
	}

	if (bPersistWhenEmpty)
	{
		Item->Quantity = 0;
		OnItemRemoved.Broadcast(ContainerId, AssetId, ItemId);
		return;
	}

	RemoveItem_InternalRemove(ContainerId, AssetId, ItemList, Item);
}



void UInventorySubsystem::OnGameLoaded()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	UInventoryStorageSubsystem* StorageSubsystem = GetGameInstance()->GetSubsystem<UInventoryStorageSubsystem>();
	if (!IsValid(StorageSubsystem))
	{
		LOG_ERROR(LogInventory, TEXT("Storage subsystem not found"));
		return;
	}

	InventoryStorage = StorageSubsystem;
}

bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventory, TEXT("InventorySubsystem initialized"));

	FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UInventorySubsystem::OnGameLoaded);
}

void UInventorySubsystem::Deinitialize()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	InventoryStorage = nullptr;

	LOG_WARNING(LogInventory, TEXT("InventorySubsystem deinitialized"));
	Super::Deinitialize();
}

