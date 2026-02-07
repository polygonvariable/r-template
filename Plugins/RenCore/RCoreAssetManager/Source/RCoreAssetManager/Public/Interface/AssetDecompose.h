// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers

// Generated Headers
#include "AssetDecompose.generated.h"

// Forward Declarations



USTRUCT(BlueprintType)
struct FAssetFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer Tags;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPrimaryAssetId> AssetId;

	UPROPERTY(EditDefaultsOnly)
	int Quantity = 1;

	friend inline bool operator == (const FAssetFragment& A, const FAssetFragment& B)
	{
		return A.Tags == B.Tags && A.AssetId == B.AssetId;
	}

	friend inline uint32 GetTypeHash(const FAssetFragment& Data)
	{
		return HashCombine(GetTypeHash(Data.AssetId), GetTypeHash(Data.Tags.ToString()));
	}

};

