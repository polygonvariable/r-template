// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "AssetDecomposeInterface.generated.h"

// Forward Declarations
class UAssetCollectionRule_Dictionary;

struct FGameplayTagContainer;



UINTERFACE(MinimalAPI)
class UAssetStructureInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCOREASSETMANAGER_API IAssetStructureInterface
{

	GENERATED_BODY()

public:

	virtual const UAssetCollectionRule_Dictionary* GetBreakdownAssets(const FGameplayTagContainer& InTags) const = 0;
	virtual const UAssetCollectionRule_Dictionary* GetRebuildAssets(const FGameplayTagContainer& InTags) const = 0;

};

