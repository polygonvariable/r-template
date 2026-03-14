// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AssetDetail.generated.h"

// Forward Declarations
class URPrimaryDataAsset;



/**
 *
 *
 */
USTRUCT(BlueprintType)
struct FAssetDetail
{

	GENERATED_BODY()

public:

	FAssetDetail() {}
	FAssetDetail(int InQuantity) : Quantity(InQuantity) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Quantity = 0;

	void Reset()
	{
		Quantity = 0;
	}

};



#if WITH_EDITORONLY_DATA

USTRUCT(BlueprintType)
struct FAssetDetail_SimpleEd : public FAssetDetail
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




/**
 *
 *
 */
USTRUCT(BlueprintType)
struct FAssetDetail_Unique : public FAssetDetail
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGuid ItemId;

	void Reset()
	{
		ItemId.Invalidate();
		FAssetDetail::Reset();
	}

};

#if WITH_EDITORONLY_DATA

USTRUCT(BlueprintType)
struct FAssetDetail_UniqueEd : public FAssetDetail_Unique
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

