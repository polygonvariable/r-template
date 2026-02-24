// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AvatarAsset.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Definition/AscensionData.h"
#include "Definition/AssetRuleDefinition.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"



FPrimaryAssetId UAvatarAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UAvatarAsset::GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType UAvatarAsset::GetPrimaryAssetType()
{
	return TEXT("Entity.Avatar");
}



int UAvatarAsset::GetExperienceInterval(int Level) const
{
	return ExperienceInterval;
}

int UAvatarAsset::GetLevelInterval(int Rank) const
{
	return LevelInterval;
}

int UAvatarAsset::GetMaxLevel() const
{
	return MaxLevel;
}

int UAvatarAsset::GetMaxRank() const
{
	return MaxRank;
}

const UAssetCollection* UAvatarAsset::GetExperienceItems(const FAscensionData& Ascension) const
{
	if (!IsValid(ExperienceItems))
	{
		return nullptr;
	}
	return ExperienceItems->GetCollectionRule<UAssetCollection>();
}

const UAssetCollection* UAvatarAsset::GetRankItems(const FAscensionData& Ascension) const
{
	if (!IsValid(RankItems))
	{
		return nullptr;
	}

	FAssetRuleContext_List Context;
	Context.Index = Ascension.Rank;

	return RankItems->GetCollectionRule<UAssetCollection>(FInstancedStruct::Make(Context));
}


