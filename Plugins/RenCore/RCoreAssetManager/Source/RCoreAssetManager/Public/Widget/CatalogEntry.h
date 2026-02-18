// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "CatalogEntry.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UCatalogEntry : public UObject
{

	GENERATED_BODY()

public:

	FGuid EntryId = FGuid();
	FPrimaryAssetId AssetId = FPrimaryAssetId();

	void InitializeData()
	{
		EntryId = FGuid::NewGuid();
	}

	virtual void ResetData()
	{
		EntryId.Invalidate();
		AssetId = FPrimaryAssetId();
	}

	// ~ UObject
	virtual void BeginDestroy() override
	{
		ResetData();
		Super::BeginDestroy();
	}
	// ~ End of UObject

};

