// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "Asset/Category/NonEnhanceableAsset.h"

// Generated Headers
#include "EnhanceAsset.generated.h"

// Forward Declarations
class UAssetGroup;



/**
 * 
 */
UCLASS(MinimalAPI)
class UEnhanceAsset : public UNonEnhanceableAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> BreakdownItems = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> RebuildItems = nullptr;


	// ~ IAssetCompositionInterface
	RINVENTORY_API virtual const UAssetCollection* GetBreakdownAssets(const FGuid& InId) const override;
	RINVENTORY_API virtual const UAssetCollection* GetRebuildAssets(const FGuid& InId) const override;
	// ~ End of IAssetCompositionInterface
	
};

