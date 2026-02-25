// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "InstancedStruct.h"

// Project Headers

// Generated Headers
#include "AssetEntry.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UAssetEntry : public UObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId AssetId = FPrimaryAssetId();
	FInstancedStruct AssetDetail;

	virtual void ResetData()
	{
		AssetId = FPrimaryAssetId();
		AssetDetail.Reset();
	}

	// ~ UObject
	virtual void BeginDestroy() override
	{
		ResetData();
		Super::BeginDestroy();
	}
	// ~ End of UObject

};

