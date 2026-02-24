// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Type/EnhanceAsset.h"

// Engine Headers

#include "InstancedStruct.h"

// Project Headers
#include "Definition/AssetRuleDefinition.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"



const UAssetCollection* UEnhanceAsset::GetBreakdownAssets(const FGameplayTagContainer& InTags) const
{
	if (!IsValid(BreakdownItems))
	{
		return nullptr;
	}
	return BreakdownItems->GetCollectionRule(FInstancedStruct::Make(FAssetRuleContext(InTags)));
}

const UAssetCollection* UEnhanceAsset::GetRebuildAssets(const FGameplayTagContainer& InTags) const
{
	if (!IsValid(RebuildItems))
	{
		return nullptr;
	}
	return RebuildItems->GetCollectionRule(FInstancedStruct::Make(FAssetRuleContext(InTags)));
}

