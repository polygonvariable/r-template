// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/Runtime/AvatarData.h"
#include "Interface/IAssetInstance.h"
#include "SaveGame/Storage.h"

// Module Macros
#define RSYSTEM_API RENTITY_API

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
class UAvatarStorage : public UStorage, public IAssetInstanceCollection
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChanged, const FPrimaryAssetId& /* AssetId */, FGuid /* ItemId */);
	FOnChanged OnAdded;
	FOnChanged OnRemoved;
	FOnChanged OnUpdated;

	DECLARE_MULTICAST_DELEGATE(FOnRefreshed);
	FOnRefreshed OnRefreshed;


	// ~ IAssetInstanceCollection
	RSYSTEM_API virtual bool AddItem(const FPrimaryAssetId& AssetId, int Quantity) override;
	RSYSTEM_API virtual bool AddItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) override;

	RSYSTEM_API virtual bool RemoveItem(const FPrimaryAssetId& AssetId, int Quantity) override;
	RSYSTEM_API virtual bool RemoveItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) override;

	RSYSTEM_API virtual bool RemoveAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) override;
	RSYSTEM_API virtual bool RemoveItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity) override;

	RSYSTEM_API virtual bool ContainItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const override;
	RSYSTEM_API virtual bool ContainAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const override;
	// ~ End of IAssetInstanceCollection


	RSYSTEM_API const FAvatarData* GetItem(const FPrimaryAssetId& AssetId) const;
	RSYSTEM_API const FAvatarData* GetItemById(const FGuid& ItemId) const;

	RSYSTEM_API bool UpdateItem(const FPrimaryAssetId& AssetId, TFunctionRef<bool(FAvatarData*)> Callback);

	RSYSTEM_API void QueryItems(const UFilterCriterion* FilterCriterion, const FAvatarQueryRule& QueryRule, TFunctionRef<void(const FAvatarSortEntry&)> Callback);

protected:

	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, FAvatarData> AvatarCollection;


	void HandleItemSorting(TArray<FAvatarSortEntry>& SortedItems, const FAvatarQueryRule& QueryRule) const;
	void QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const;
	void QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const;


};


// Module Macros
#undef RSYSTEM_API

