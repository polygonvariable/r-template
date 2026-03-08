// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/InventoryStorage.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Definition/AssetFilterProperty.h"
#include "Definition/FilterContext.h"
#include "Definition/InventoryFilterProperty.h"
#include "Definition/InventoryQueryType.h"
#include "Definition/InventorySortType.h"
#include "Filter/FilterCriterion.h"
#include "Library/InventoryPrimaryAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"



bool UInventoryStorage::AddItem(const FPrimaryAssetId& AssetId, int Quantity)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);
	return AddItem_Internal(AssetId, Quantity, Stack);
}

bool UInventoryStorage::AddItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		FInventoryStack* Stack = FindOrAddStack(AssetId);
		AddItem_Internal(AssetId, Quantity, Stack);
	}

	return true;
}



bool UInventoryStorage::RemoveItem(const FPrimaryAssetId& AssetId, int Quantity)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);

	return RemoveItem_Internal(AssetId, Quantity, Stack);
}

bool UInventoryStorage::RemoveItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	if (!ContainItems(Items, Multiplier))
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	bool bResult = true;

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		FInventoryStack* Stack = FindOrAddStack(AssetId);
		bool bRemoved = RemoveItem_Internal(AssetId, Quantity, Stack);
		if (!bRemoved)
		{
			bResult = false;
			break;
		}
	}

	return true;
}

bool UInventoryStorage::RemoveAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity)
{
	FPrimaryAssetId SelectedAssetId;
	int SelectedQuantity = 0;

	if (!ContainAnyItems(InItems, InMultiplier, SelectedAssetId, SelectedQuantity))
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	bool bRemoved = RemoveItem(SelectedAssetId, SelectedQuantity);
	if (!bRemoved)
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	OutAssetId = SelectedAssetId;
	OutQuantity = SelectedQuantity;
	return true;
}

bool UInventoryStorage::RemoveItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);
	bool bRemoved = RemoveItemById_Internal(AssetId, ItemId, Quantity, Stack);

	return bRemoved;
}



bool UInventoryStorage::UpdateItem(const FPrimaryAssetId& AssetId, TFunctionRef<void(FInventoryItem*)> Callback)
{
	FInventoryItem* Item = GetMutableItemByIndex(AssetId, 0);
	if (!Item)
	{
		return false;
	}

	Callback(Item);

	OnItemUpdated.Broadcast(AssetId, Item->ItemId);
	OnInventoryRefreshed.Broadcast();

	return true;
}

bool UInventoryStorage::UpdateItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId, TFunctionRef<void(FInventoryItem*)> Callback)
{
	FInventoryItem* Item = GetMutableItemById(AssetId, ItemId);
	if (!Item)
	{
		return false;
	}

	Callback(Item);

	OnItemUpdated.Broadcast(AssetId, Item->ItemId);
	OnInventoryRefreshed.Broadcast();

	return true;
}



bool UInventoryStorage::ContainItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const
{
	bool bResult = true;

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int RequiredQuantity = Kv.Value * Multiplier;

		if (!InventoryPrimaryAsset::IsValid(AssetId) || RequiredQuantity <= 0)
		{
			bResult = false;
			break;
		}

		const FInventoryStack* Stack = GetStack(AssetId);
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

bool UInventoryStorage::ContainAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const
{
	bool bResult = false;

	for (const TPair<FPrimaryAssetId, int>& Kv : InItems)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int RequiredQuantity = Kv.Value * InMultiplier;

		if (!InventoryPrimaryAsset::IsValid(AssetId) || RequiredQuantity <= 0)
		{
			continue;
		}

		const FInventoryStack* Stack = GetStack(AssetId);
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



int UInventoryStorage::GetTotalQuantity(const FPrimaryAssetId& AssetId) const
{
	return GetItemQuantity_Internal(AssetId);
}

const FInventoryItem* UInventoryStorage::GetItem(const FPrimaryAssetId& AssetId) const
{
	const FInventoryStack* Stack = GetStack(AssetId);
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

const FInventoryItem* UInventoryStorage::GetItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId) const
{
	const FInventoryStack* Stack = GetStack(AssetId);
	if (!Stack)
	{
		return nullptr;
	}

	const TArray<FInventoryItem>& RecordList = Stack->ItemList;
	const FInventoryItem* Record = RecordList.FindByPredicate([ItemId](const FInventoryItem& Record) { return Record.ItemId == ItemId; });

	return Record;
}



void UInventoryStorage::QueryItems(const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		return;
	}

	if (QueryRule.QuerySource == EInventoryQuerySource::Glossary)
	{
		HandleGlossaryItems(AssetManager, FilterCriterion, QueryRule, MoveTemp(Callback));
	}
	else
	{
		HandleInventoryItems(AssetManager, FilterCriterion, QueryRule, MoveTemp(Callback));
	}
}




FInventoryItem* UInventoryStorage::GetMutableItemByIndex(const FPrimaryAssetId& AssetId, int Index)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);
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

FInventoryItem* UInventoryStorage::GetMutableItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);
	if (!Stack)
	{
		return nullptr;
	}

	TArray<FInventoryItem>& ItemList = Stack->ItemList;
	FInventoryItem* Item = ItemList.FindByPredicate([ItemId](const FInventoryItem& Record) { return Record.ItemId == ItemId; });

	return Item;
}



void UInventoryStorage::HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const
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

void UInventoryStorage::HandleGlossaryItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const
{
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

		int ItemQuantity = GetItemQuantity_Internal(AssetId);

		SortedItems.Emplace(AssetId, DisplayName, ItemQuantity);
	}

	HandleItemSorting(SortedItems, QueryRule);

	for (const FInventorySortEntry& Item : SortedItems)
	{
		Callback(Item);
	}
}

void UInventoryStorage::HandleInventoryItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const
{
	TArray<FInventorySortEntry> SortedItems;

	for (const TPair<FPrimaryAssetId, FInventoryStack>& Kv : InventoryStack)
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




const FInventoryStack* UInventoryStorage::GetStack(const FPrimaryAssetId& AssetId) const
{
	return InventoryStack.Find(AssetId);
}

FInventoryStack* UInventoryStorage::FindOrAddStack(const FPrimaryAssetId& AssetId)
{
	FInventoryStack* Stack = InventoryStack.Find(AssetId);
	if (Stack)
	{
		return Stack;
	}

	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		return nullptr;
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
	FInventoryStack& NewStack = InventoryStack.Add(AssetId, { StackId, bStackable, bPersistWhenEmpty });

	return &NewStack;
}



int UInventoryStorage::GetItemQuantity_Internal(const FPrimaryAssetId& AssetId) const
{
	const FInventoryStack* Stack = GetStack(AssetId);
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



bool UInventoryStorage::AddItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
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
			AddItem_InternalUpdate(AssetId, ItemData, Quantity);
			return true;
		}
		else
		{
			AddItem_InternalCreate(AssetId, ItemList, Quantity);
			return true;
		}
	}

	for (int i = 0; i < Quantity; i++)
	{
		AddItem_InternalCreate(AssetId, ItemList, 1);
	}

	return true;
}

void UInventoryStorage::AddItem_InternalCreate(const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity)
{
	FInventoryItem Item;
	Item.ItemId = FGuid::NewGuid();
	Item.Quantity = Quantity;
	ItemList.Add(Item);

	OnItemAdded.Broadcast(AssetId, Item.ItemId);
	OnInventoryRefreshed.Broadcast();

	LOG_INFO(LogInventory, TEXT("New item added"));
}

void UInventoryStorage::AddItem_InternalUpdate(const FPrimaryAssetId& AssetId, FInventoryItem& Item, int Quantity)
{
	Item.Quantity += Quantity;

	OnItemAdded.Broadcast(AssetId, Item.ItemId);
	OnInventoryRefreshed.Broadcast();

	LOG_INFO(LogInventory, TEXT("Item updated"));
}



bool UInventoryStorage::RemoveItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
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
			RemoveItem_InternalUpdate(AssetId, ItemList, bPersist, &Record, Quantity);

			return true;
		}

		LOG_ERROR(LogInventory, TEXT("Not item to remove"));
		return false;
	}

	bool bRemoved = RemoveItem_InternalStack(AssetId, ItemList, Quantity);
	return bRemoved;
}

bool UInventoryStorage::RemoveItemById_Internal(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity, FInventoryStack* Stack)
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
		RemoveItem_InternalUpdate(AssetId, ItemList, bPersistWhenEmpty, Item, Quantity);
		return true;
	}

	bool bRemoved = RemoveItem_InternalRemove(AssetId, ItemList, Item);
	return bRemoved;
}

bool UInventoryStorage::RemoveItem_InternalStack(const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity)
{
	int StackCount = ItemList.Num();
	if (Quantity > StackCount)
	{
		return false;
	}

	for (int i = 0; i < Quantity; i++)
	{
		FInventoryItem Item = ItemList.Pop();

		OnItemRemoved.Broadcast(AssetId, Item.ItemId);
		OnInventoryRefreshed.Broadcast();
	}

	return true;
}

bool UInventoryStorage::RemoveItem_InternalRemove(const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, FInventoryItem* Item)
{
	FInventoryItem TempItem = *Item;

	int Count = ItemList.Remove(TempItem);
	if (Count == 0)
	{
		return false;
	}

	OnItemRemoved.Broadcast(AssetId, TempItem.ItemId);
	OnInventoryRefreshed.Broadcast();

	return true;
}

void UInventoryStorage::RemoveItem_InternalUpdate(const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, bool bPersistWhenEmpty, FInventoryItem* Item, int Quantity)
{
	FGuid ItemId = Item->ItemId;
	int ItemQuantity = Item->Quantity;

	int NewQuantity = ItemQuantity - Quantity;
	if (NewQuantity > 0)
	{
		Item->Quantity = NewQuantity;

		OnItemRemoved.Broadcast(AssetId, ItemId);
		OnInventoryRefreshed.Broadcast();

		return;
	}

	if (bPersistWhenEmpty)
	{
		Item->Quantity = 0;

		OnItemRemoved.Broadcast(AssetId, ItemId);
		OnInventoryRefreshed.Broadcast();

		return;
	}

	RemoveItem_InternalRemove(AssetId, ItemList, Item);
}

