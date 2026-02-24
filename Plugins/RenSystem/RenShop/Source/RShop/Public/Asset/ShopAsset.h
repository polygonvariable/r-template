// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"

// Generated Headers
#include "ShopAsset.generated.h"

// Forward Declarations
class UAssetGroup;



/**
 *
 */
UCLASS()
class UShopAsset : public URPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGuid ShopId;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> ShopItems = nullptr;

	// ~ UObject
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UObject

};

