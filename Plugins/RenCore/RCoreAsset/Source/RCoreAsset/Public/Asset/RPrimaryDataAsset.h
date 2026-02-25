// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers

// Generated Headers
#include "RPrimaryDataAsset.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class URPrimaryDataAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

};

