// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers

// Generated Headers

// Forward Declarations



struct FAssetRuleContext
{

public:

	FAssetRuleContext() {}
	FAssetRuleContext(const FGameplayTagContainer& InTags) : Tags(InTags) {}

	FGameplayTagContainer Tags;

};

struct FAssetRuleContext_List : public FAssetRuleContext
{

public:

	int Index = 0;
	
};

