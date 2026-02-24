// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "AssetStructureInterface.generated.h"

// Module Macros
#define RCORE_API RCOREASSETMANAGER_API

// Forward Declarations
class UAssetCollection;

struct FGameplayTagContainer;



UINTERFACE(MinimalAPI)
class UAssetStructureInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORE_API IAssetStructureInterface
{

	GENERATED_BODY()

public:

	virtual const UAssetCollection* GetBreakdownAssets(const FGameplayTagContainer& InTags) const = 0;
	virtual const UAssetCollection* GetRebuildAssets(const FGameplayTagContainer& InTags) const = 0;

};



// Module Macros
#undef RCORE_API

