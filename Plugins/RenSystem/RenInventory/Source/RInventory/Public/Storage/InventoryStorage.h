// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/Runtime/InventoryStack.h"
#include "Interface/IAscensionInstanceData.h"
#include "Interface/IAssetInstanceCollection.h"
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
class UInventoryStorage : public UStorage, public IAssetInstanceCollection, public IAscensionInstanceData
{

	GENERATED_BODY()

public:

	// ~ UStorage
	RSYSTEM_API virtual void InitializeDefaults() override;
	// ~ End of UStorage

	// ~ IAssetInstanceCollection
	RSYSTEM_API virtual bool AddInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	RSYSTEM_API virtual bool AddInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) override;

	RSYSTEM_API virtual bool RemoveInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	RSYSTEM_API virtual bool RemoveInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) override;

	RSYSTEM_API virtual bool RemoveAnyInstances(const TMap<FPrimaryAssetId, int>& InAssetIds, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) override;
	RSYSTEM_API virtual bool RemoveInstanceById(const FPrimaryAssetId& AssetId, const FGuid& InstanceId, int Quantity) override;

	RSYSTEM_API virtual bool ContainInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) const override;
	RSYSTEM_API virtual bool ContainAnyInstances(const TMap<FPrimaryAssetId, int>& InAssetIds, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const override;

	virtual FOnAssetInstanceCollectionUpdated& GetOnAssetInstanceCollectionUpdated() override;
	// ~ End of IAssetInstanceCollection

	// ~ IAscensionInstanceData
	virtual const FAscensionData* GetAscensionInstance(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const override;
	// ~ End of IAscensionInstanceData

	RSYSTEM_API bool UpdateInstance(const FPrimaryAssetId& AssetId, TFunctionRef<void(FInventoryInstance*)> Callback);
	RSYSTEM_API bool UpdateInstanceById(const FPrimaryAssetId& AssetId, const FGuid& InstanceId, TFunctionRef<void(FInventoryInstance*)> Callback);

	RSYSTEM_API int GetTotalQuantity(const FPrimaryAssetId& AssetId) const;
	RSYSTEM_API const FInventoryInstance* GetInstance(const FPrimaryAssetId& AssetId) const;
	RSYSTEM_API const FInventoryInstance* GetInstanceById(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const;

	RSYSTEM_API void QueryInstances(const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback);

protected:

	FOnAssetInstanceCollectionUpdated OnCollectionUpdated;

	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, FInventoryStack> InventoryStack;


	FInventoryInstance* GetMutableItemByIndex(const FPrimaryAssetId& AssetId, int Index);
	FInventoryInstance* GetMutableItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId);

	void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;
	void QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FInventorySortEntry>& OutSortedItems) const;
	void QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FInventorySortEntry>& OutSortedItems) const;

	const FInventoryStack* GetStack(const FPrimaryAssetId& AssetId) const;
	FInventoryStack* FindOrAddStack(const FPrimaryAssetId& AssetId);

	int GetItemQuantity_Internal(const FPrimaryAssetId& AssetId) const;

	bool AddItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	void AddItem_InternalCreate(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, int Quantity);
	void AddItem_InternalUpdate(const FPrimaryAssetId& AssetId, FInventoryInstance& Record, int Quantity);

	bool RemoveItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	bool RemoveItemById_Internal(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity, FInventoryStack* Stack);
	bool RemoveItem_InternalStack(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, int Quantity);
	bool RemoveItem_InternalRemove(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, FInventoryInstance* Item);
	void RemoveItem_InternalUpdate(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, bool bPersistWhenEmpty, FInventoryInstance* Item, int Quantity);

};



// Module Macros
#undef RSYSTEM_API

