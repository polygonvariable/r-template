// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers
#include "Interface/AssetDecomposeInterface.h"
#include "Definition/InventoryItemRarity.h"
#include "Definition/InventoryItemType.h"

// Generated Headers
#include "InventoryAsset.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryAsset : public UPrimaryDataAsset, public IAssetStructureInterface
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

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
	RCOREINVENTORY_API virtual const UAssetCollection* GetBreakdownAssets(const FGameplayTagContainer& InTags) const override;
	RCOREINVENTORY_API virtual const UAssetCollection* GetRebuildAssets(const FGameplayTagContainer& InTags) const override;
	// ~ End of IAssetStructureInterface

	// ~ UPrimaryDataAsset
	RCOREINVENTORY_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

public:

	RCOREINVENTORY_API static FPrimaryAssetType GetPrimaryAssetType();

};

