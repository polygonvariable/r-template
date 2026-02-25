// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers

// Generated Headers
#include "AssetRuleDefinition.generated.h"

// Forward Declarations



USTRUCT()
struct FAssetRuleContext
{

	GENERATED_BODY()

public:

	FAssetRuleContext() {}
	FAssetRuleContext(const FGameplayTagContainer& InTags) : Tags(InTags) {}

	FGameplayTagContainer Tags;

};

USTRUCT()
struct FAssetRuleContext_List : public FAssetRuleContext
{

	GENERATED_BODY()

public:

	int Index = 0;
	
};

