// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Definition/InventoryItemRarity.h"
#include "Definition/InventoryItemType.h"
#include "Interface/AssetStructureInterface.h"

// Generated Headers
#include "InventoryAsset.generated.h"

// Module Macros
#define RCORE_API RCOREINVENTORY_API

// Forward Declarations



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryAsset : public URPrimaryDataAsset, public IAssetStructureInterface
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	EInventoryItemType Type = EInventoryItemType::Default;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	EInventoryItemRarity Rarity = EInventoryItemRarity::Default;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	bool bStackable = false;

	/**
	 * When true, the item will still be present in inventory even if the quantity is 0,
	 * this can be useful in things like bullets, as it can avoid constant adding &
	 * removal of key value pairs.
	 */
	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	bool bPersistWhenEmpty = false;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	bool bExpires = false;

	UPROPERTY(EditDefaultsOnly)
	FTimespan ExpirationDuration = FTimespan::Zero();


	// ~ IAssetStructureInterface
	RCORE_API virtual const UAssetCollection* GetBreakdownAssets(const FGameplayTagContainer& InTags) const override;
	RCORE_API virtual const UAssetCollection* GetRebuildAssets(const FGameplayTagContainer& InTags) const override;
	// ~ End of IAssetStructureInterface

	// ~ UPrimaryDataAsset
	RCORE_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

public:

	RCORE_API static FPrimaryAssetType GetPrimaryAssetType();

};



// Module Macros
#undef RCORE_API

