// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/TransactionalAsset.h"
#include "Definition/InventoryItemRarity.h"
#include "Definition/InventoryItemType.h"
#include "Interface/AssetStructureInterface.h"

// Generated Headers
#include "InventoryAsset.generated.h"

// Module Macros
#define RSYSTEM_API RINVENTORY_API

// Forward Declarations



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryAsset : public UTransactionalAsset, public IAssetStructureInterface
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
	RSYSTEM_API virtual const UAssetCollection* GetBreakdownAssets(const FGameplayTagContainer& InTags) const override;
	RSYSTEM_API virtual const UAssetCollection* GetRebuildAssets(const FGameplayTagContainer& InTags) const override;
	// ~ End of IAssetStructureInterface

	// ~ UPrimaryDataAsset
	RSYSTEM_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

public:

	RSYSTEM_API static FPrimaryAssetType GetPrimaryAssetType();

};



// Module Macros
#undef RSYSTEM_API

