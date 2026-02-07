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

	FGameplayTagContainer RuleTags;

};

struct FAssetRuleContext_Sequence : public FAssetRuleContext
{

public:

	int Index = 0;
	
};
