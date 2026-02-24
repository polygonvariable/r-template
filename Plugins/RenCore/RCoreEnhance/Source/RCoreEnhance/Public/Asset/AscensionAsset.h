// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"

// Generated Headers
#include "AscensionAsset.generated.h"

// Forward Declarations


/**
 *
 *
 *
 */
UCLASS(MinimalAPI)
class UMetadataAsset : public URPrimaryDataAsset
{

	GENERATED_BODY()

public:

	// ~ UPrimaryDataAsset
	RCOREENHANCE_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

};

/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UExperiencePointAsset : public UMetadataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	int Points = 0;

	RCOREENHANCE_API virtual int GetPoints(int Quality) const;

};

