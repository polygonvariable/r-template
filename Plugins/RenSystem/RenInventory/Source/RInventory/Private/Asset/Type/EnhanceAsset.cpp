// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Type/EnhanceAsset.h"

// Engine Headers

// Project Headers
#include "Asset/AssetRuleDefinition.h"
#include "AssetCollection.h"
#include "Definition/AscensionData.h"



const UAssetCollection* UEnhanceAsset::GetBreakdownAssets(const FGameplayTagContainer& InTags) const
{
	return BreakdownItems->GetCollectionRule<UAssetCollection>(FAssetRuleContext(InTags));
}

const UAssetCollection* UEnhanceAsset::GetRebuildAssets(const FGameplayTagContainer& InTags) const
{
	return RebuildItems->GetCollectionRule<UAssetCollection>(FAssetRuleContext(InTags));
}

