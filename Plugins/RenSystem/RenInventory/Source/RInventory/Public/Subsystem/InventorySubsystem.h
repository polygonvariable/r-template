// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers

// Generated Headers
#include "InventorySubsystem.generated.h"

// Module Macros
#ifdef RSYSTEM_API
#undef RSYSTEM_API
#endif
#define RSYSTEM_API RINVENTORY_API

// Forward Declarations
class UStorage;
class URAssetManager;
class UAssetManager;
class UFilterCriterion;
class UInventoryAsset;
class UInventoryStorage;
class IInventoryProviderInterface;
class IStorageProviderInterface;

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

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemChanged, FGuid /* InventoryId */, const FPrimaryAssetId& /* AssetId */, FGuid /* ItemId */);
	FOnInventoryItemChanged OnItemAdded;
	FOnInventoryItemChanged OnItemRemoved;
	FOnInventoryItemChanged OnItemUpdated;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryRefresh, const FGuid& /* InventoryId */);
	FOnInventoryRefresh OnInventoryRefreshed;


	RSYSTEM_API bool AddItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Quantity);
	RSYSTEM_API bool AddItems(const FGuid& InventoryId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier);

	/*
	 * Removes an item from the stack within the specified container.
	 * Useful when the exact itemId is not required.
	 */
	RSYSTEM_API bool RemoveItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Quantity);
	RSYSTEM_API bool RemoveItems(const FGuid& InventoryId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier);

	/** Removes any first valid item from the item list.
	 * 
	 * For stackable items, the quantity is reduced from the item,
	 * and for non-stackable items, the number of item will be removed from the stack.
	 * Useful when the exact itemId is not required.
	 */
	RSYSTEM_API bool RemoveAnyItems(const FGuid& InContainerId, const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity);
	RSYSTEM_API bool RemoveItemById(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity);

	RSYSTEM_API bool UpdateItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TFunctionRef<void(FInventoryItem*)> Callback);
	RSYSTEM_API bool UpdateItemById(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, TFunctionRef<void(FInventoryItem*)> Callback);

	RSYSTEM_API bool ContainItems(const FGuid& InventoryId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const;
	RSYSTEM_API bool ContainAnyItems(const FGuid& InContainerId, const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const;


	RSYSTEM_API int GetTotalQuantity(const FGuid& InventoryId, const FPrimaryAssetId& AssetId) const;
	RSYSTEM_API const FInventoryItem* GetItemById(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId) const;

	RSYSTEM_API void QueryItems(const FGuid& InventoryId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback);


protected:

	TWeakInterfacePtr<IStorageProviderInterface> StorageProvider;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;


	FInventoryItem* GetMutableItemByIndex(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Index) const;
	FInventoryItem* GetMutableItemById(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId) const;


	virtual void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;
	virtual void HandleGlossaryItems(const FGuid& InventoryId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const;
	virtual void HandleInventoryItems(const FGuid& InventoryId, const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const;


	UInventoryStorage* GetStorage(const FGuid& InventoryId) const;
	TMap<FPrimaryAssetId, FInventoryStack>* GetStackCollection(UInventoryStorage* Storage) const;
	FInventoryStack* GetStack(const FPrimaryAssetId& AssetId, UInventoryStorage* Storage) const;
	FInventoryStack* FindOrAddStack(const FPrimaryAssetId& AssetId, UInventoryStorage* Storage);


	int GetItemQuantity_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, UInventoryStorage* Storage) const;

	bool AddItem_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	void AddItem_InternalCreate(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity);
	void AddItem_InternalUpdate(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, FInventoryItem& Record, int Quantity);


	bool RemoveItem_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	bool RemoveItemById_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity, FInventoryStack* Stack);
	bool RemoveItem_InternalStack(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, int Quantity);
	bool RemoveItem_InternalRemove(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, FInventoryItem* Item);
	void RemoveItem_InternalUpdate(const FGuid& InventoryId, const FPrimaryAssetId& AssetId, TArray<FInventoryItem>& ItemList, bool bPersistWhenEmpty, FInventoryItem* Item, int Quantity);


	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem


public:

	static RSYSTEM_API UInventorySubsystem* Get(UWorld* World);
	RSYSTEM_API static UInventorySubsystem* Get(UGameInstance* GameInstance);

	RSYSTEM_API static FGuid GetStorageId();
	RSYSTEM_API static FString GetStorageUrl();
	RSYSTEM_API static TSubclassOf<UStorage> GetStorageClass();

};



// Module Macros
#undef RSYSTEM_API

