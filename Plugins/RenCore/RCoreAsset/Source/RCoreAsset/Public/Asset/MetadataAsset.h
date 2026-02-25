// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"

// Generated Headers
#include "MetadataAsset.generated.h"

// Module Macros
#define RCORE_API RCOREASSET_API

// Forward Declarations


/**
 *
 *
 *
 */
UCLASS(Abstract, MinimalAPI)
class UMetadataAsset : public URPrimaryDataAsset
{

	GENERATED_BODY()

public:

	// ~ UPrimaryDataAsset
	RCORE_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset


};



// Module Macros
#undef RCORE_API

