// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers

// Generated Headers
#include "RPrimaryAsset.generated.h"

// Forward Declarations



/**
 *
 *
 */
UCLASS(MinimalAPI)
class URPrimaryAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(VisibleDefaultsOnly, AssetRegistrySearchable)
	FName Type = FName();

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

};

