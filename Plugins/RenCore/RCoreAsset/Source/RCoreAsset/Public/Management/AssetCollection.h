// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers

// Generated Headers
#include "AssetCollection.generated.h"

// Module Macros
#define RCORE_API RCOREASSET_API

// Forward Declarations
class URPrimaryDataAsset;



/**
 *
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UAssetCollection : public UObject
{

	GENERATED_BODY()

public:

	RCORE_API const FGameplayTagContainer& GetCollectionTags() const;

protected:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer Tags;

};



// Module Macros
#undef RCORE_API

