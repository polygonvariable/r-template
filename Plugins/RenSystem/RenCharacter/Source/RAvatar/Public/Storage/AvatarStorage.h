// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/Runtime/AvatarInstance.h"
#include "Interface/IAscensionInstanceData.h"
#include "Interface/IAssetInstanceCollection.h"
#include "SaveGame/Storage.h"

// Module Macros
#define RSYSTEM_API RAVATAR_API

// Generated Headers
#include "AvatarStorage.generated.h"

// Forward Declarations
class UAssetManager;
class UFilterCriterion;

struct FAvatarQueryRule;
struct FAvatarSortEntry;



/**
 *
 */
UCLASS(MinimalAPI)
class UAvatarStorage : public UStorage, public IAssetInstanceCollection, public IAscensionInstanceData
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

	RSYSTEM_API const FAvatarInstance* GetInstance(const FPrimaryAssetId& AssetId) const;
	RSYSTEM_API const FAvatarInstance* GetInstanceById(const FGuid& InstanceId) const;

	RSYSTEM_API bool UpdateInstance(const FPrimaryAssetId& AssetId, TFunctionRef<void(FAvatarInstance*)> Callback);
	RSYSTEM_API void QueryInstances(const UFilterCriterion* FilterCriterion, const FAvatarQueryRule& QueryRule, TFunctionRef<void(const FAvatarSortEntry&)> Callback);

protected:

	FOnAssetInstanceCollectionUpdated OnCollectionUpdated;

	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, FAvatarInstance> AvatarCollection;


	void HandleItemSorting(TArray<FAvatarSortEntry>& SortedItems, const FAvatarQueryRule& QueryRule) const;
	void QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const;
	void QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const;

	bool AddInstance_Internal(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId);
	bool RemoveInstance_Internal(const FPrimaryAssetId& AssetId);

};


// Module Macros
#undef RSYSTEM_API

