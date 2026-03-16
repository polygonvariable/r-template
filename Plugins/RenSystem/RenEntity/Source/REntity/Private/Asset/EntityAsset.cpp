// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/EntityAsset.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Definition/AscensionData.h"
#include "Definition/AssetRuleDefinition.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"



FPrimaryAssetId UEntityAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UEntityAsset::GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType UEntityAsset::GetPrimaryAssetType()
{
	return TEXT("Asset.Entity");
}



int UEntityAsset::GetExperienceInterval(int Level) const
{
	return ExperienceInterval;
}

int UEntityAsset::GetLevelInterval(int Rank) const
{
	return LevelInterval;
}

int UEntityAsset::GetMaxLevel() const
{
	return MaxLevel;
}

int UEntityAsset::GetMaxRank() const
{
	return MaxRank;
}

const UAssetCollection* UEntityAsset::GetExperienceAssets(const FAscensionData& Ascension) const
{
	if (!IsValid(LevelUpAssets))
	{
		return nullptr;
	}
	return LevelUpAssets->GetCollectionRule<UAssetCollection>();
}

const UAssetCollection* UEntityAsset::GetRankAssets(const FAscensionData& Ascension) const
{
	if (!IsValid(RankUpAssets))
	{
		return nullptr;
	}

	FAssetRuleContext_List Context;
	Context.Index = Ascension.Rank;

	return RankUpAssets->GetCollectionRule<UAssetCollection>(FInstancedStruct::Make(Context));
}


