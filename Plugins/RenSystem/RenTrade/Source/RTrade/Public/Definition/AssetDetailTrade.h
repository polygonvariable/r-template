// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AssetDetail.h"

// Generated Headers
#include "AssetDetailTrade.generated.h"

// Forward Declarations
class URPrimaryDataAsset;



/**
 *
 *
 */
USTRUCT(BlueprintType)
struct FAssetDetail_Trade : public FAssetDetail
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	int Quota = 0;

	void Reset()
	{
		FAssetDetail::Reset();
	}

};

#if WITH_EDITORONLY_DATA

USTRUCT(BlueprintType)
struct FAssetDetail_TradeEd : public FAssetDetail_Trade
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<URPrimaryDataAsset> DataAsset;

	void Reset()
	{
		DataAsset.Reset();
		FAssetDetail::Reset();
	}

};

#endif

