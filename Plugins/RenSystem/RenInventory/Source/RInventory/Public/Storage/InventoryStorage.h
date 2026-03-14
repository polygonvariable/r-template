// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/Runtime/InventoryStack.h"
#include "Interface/IAssetInstance.h"
#include "SaveGame/Storage.h"

// Generated Headers
#include "InventoryStorage.generated.h"

// Module Macros
#define RSYSTEM_API RINVENTORY_API

// Forward Declarations
class UAssetManager;
class UFilterCriterion;

struct FInventoryQueryRule;
struct FInventorySortEntry;



/**
 *
 */
UCLASS(MinimalAPI)
class UInventoryStorage : public UStorage, public IAssetInstanceCollection
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryUpdated, const FPrimaryAssetId& /* AssetId */, FGuid /* ItemId */);
	FOnInventoryUpdated OnItemAdded;
	FOnInventoryUpdated OnItemRemoved;
	FOnInventoryUpdated OnItemUpdated;

	DECLARE_MULTICAST_DELEGATE(FOnInventoryRefreshed);
	FOnInventoryRefreshed OnInventoryRefreshed;


	// ~ IAssetInstanceCollection
	RSYSTEM_API virtual bool AddItem(const FPrimaryAssetId& AssetId, int Quantity) override;
	RSYSTEM_API virtual bool AddItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) override;

	/*
	 * Removes an item from the stack within the specified container.
	 * Useful when the exact itemId is not required.
	 */
	RSYSTEM_API virtual bool RemoveItem(const FPrimaryAssetId& AssetId, int Quantity) override;
	RSYSTEM_API virtual bool RemoveItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) override;

	/** Removes any first valid item from the item list.
	 *
	 * For stackable items, the quantity is reduced from the item,
	 * and for non-stackable items, the number of item will be removed from the stack.
	 * Useful when the exact itemId is not required.
	 */
	RSYSTEM_API virtual bool RemoveAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) override;
	RSYSTEM_API virtual bool RemoveItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity) override;

	RSYSTEM_API virtual bool ContainItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const override;
	RSYSTEM_API virtual bool ContainAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const override;
	// ~ End of IAssetInstanceCollection

	RSYSTEM_API bool UpdateItem(const FPrimaryAssetId& AssetId, TFunctionRef<void(FInventoryItem*)> Callback);
	RSYSTEM_API bool UpdateItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId, TFunctionRef<void(FInventoryItem*)> Callback);

	RSYSTEM_API int GetTotalQuantity(const FPrimaryAssetId& AssetId) const;
	RSYSTEM_API const FInventoryItem* GetItem(const FPrimaryAssetId& AssetId) const;
	RSYSTEM_API const FInventoryItem* GetItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId) const;

	RSYSTEM_API void QueryItems(const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback);

protected:

	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, FInventoryStack> InventoryStack;


	FInventoryItem* GetMutableItemByIndex(const FPrimaryAssetId& AssetId, int Index);
	FInventoryItem* GetMutableItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId);

	void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;
	void QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FInventorySortEntry>& OutSortedItems) const;
	void QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FInventorySortEntry>& OutSortedItems) const;

	const FInventoryStack* GetStack(const FPrimaryAssetId& AssetId) const;
	FInventoryStack* FindOrAddStack(const FPrimaryAssetId& AssetId);

	int GetItemQuantity_Internal(const FPrimaryAssetId& AssetId) const;

	bool AddItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	void AddItem_InternalCreate(const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity);
	void AddItem_InternalUpdate(const FPrimaryAssetId& AssetId, FInventoryItem& Record, int Quantity);

	bool RemoveItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	bool RemoveItemById_Internal(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity, FInventoryStack* Stack);
	bool RemoveItem_InternalStack(const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity);
	bool RemoveItem_InternalRemove(const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, FInventoryItem* Item);
	void RemoveItem_InternalUpdate(const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, bool bPersistWhenEmpty, FInventoryItem* Item, int Quantity);

};



// Module Macros
#undef RSYSTEM_API

