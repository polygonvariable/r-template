// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "InventoryManagerInterface.h"

// Generated Headers
#include "InventorySubsystem.generated.h"

// Forward Declarations
class URAssetManager;
class UAssetManager;

class UFilterCriterion;
class UInventoryAsset;
class IInventoryProviderInterface;
class UInventoryStorageSubsystem;
class UInventoryStorage;

struct FInventoryItem;
struct FInventoryStack;
struct FInventoryQueryRule;
struct FInventorySortEntry;



/**
 * TODO:
 * - Make inventory storage separate,
 * 	 where each container is saved separately.
 *
 * - Add support for instanced items to support items like bullets etc.
 *
 */
UCLASS(MinimalAPI)
class UInventorySubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemChanged, FGuid /* ContainerId */, const FPrimaryAssetId& /* AssetId */, FGuid /* ItemId */);
	FOnInventoryItemChanged OnItemAdded;
	FOnInventoryItemChanged OnItemRemoved;
	FOnInventoryItemChanged OnItemUpdated;


	RINVENTORY_API bool AddItem(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, int Quantity);
	RINVENTORY_API bool AddItems(const FGuid& ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier);

	/*
	 * Removes an item from the stack within the specified container.
	 * Useful when the exact itemId is not required.
	 */
	RINVENTORY_API bool RemoveItem(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, int Quantity);
	RINVENTORY_API bool RemoveItems(const FGuid& ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier);

	/** Removes any first valid item from the item list.
	 * 
	 * For stackable items, the quantity is reduced from the item,
	 * and for non-stackable items, the number of item will be removed from the stack.
	 * Useful when the exact itemId is not required.
	 */
	RINVENTORY_API bool RemoveAnyItems(const FGuid& InContainerId, const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity);
	RINVENTORY_API bool RemoveItemById(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity);

	RINVENTORY_API bool ContainItems(const FGuid& ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const;
	RINVENTORY_API bool ContainAnyItems(const FGuid& InContainerId, const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const;


	RINVENTORY_API int GetTotalQuantity(const FGuid& ContainerId, const FPrimaryAssetId& AssetId) const;
	RINVENTORY_API const FInventoryItem* GetItemById(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, const FGuid& ItemId) const;

	RINVENTORY_API void QueryItems(const FGuid& ContainerId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback);


protected:

	UPROPERTY()
	TObjectPtr<UInventoryStorageSubsystem> InventoryStorage;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;


	virtual void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;
	virtual void HandleGlossaryItems(const FGuid& ContainerId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const;
	virtual void HandleInventoryItems(const FGuid& ContainerId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const;


	UInventoryStorage* GetStorage(const FGuid& ContainerId) const;
	TMap<FPrimaryAssetId, FInventoryStack>* GetStackCollection(UInventoryStorage* Storage) const;
	FInventoryStack* GetStack(const FPrimaryAssetId& AssetId, UInventoryStorage* Storage) const;
	FInventoryStack* FindOrAddStack(const FPrimaryAssetId& AssetId, UInventoryStorage* Storage);


	int GetItemQuantity_Internal(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, UInventoryStorage* Storage) const;

	bool AddItem_Internal(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	void AddItem_InternalCreate(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity);
	void AddItem_InternalUpdate(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, FInventoryItem& Record, int Quantity);


	bool RemoveItem_Internal(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	bool RemoveItemById_Internal(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity, FInventoryStack* Stack);
	bool RemoveItem_InternalStack(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity);
	bool RemoveItem_InternalRemove(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, FInventoryItem* Item);
	void RemoveItem_InternalUpdate(const FGuid& ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, bool bPersistWhenEmpty, FInventoryItem* Item, int Quantity);


	void OnGameLoaded();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};