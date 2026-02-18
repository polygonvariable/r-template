// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AvatarAsset.h"

// Engine Headers

// Project Headers



const UAssetCollection* UAvatarAsset::GetExperienceItems(int InExperience, int InLevel, int InRank) const
{
	if (!IsValid(ExperienceItems))
	{
		return nullptr;
	}

	return ExperienceItems->GetCollectionRule<UAssetCollection>();
}


const UAssetCollection* UAvatarAsset::GetRankItems(int InExperience, int InLevel, int InRank) const
{
	if (!IsValid(RankItems))
	{
		return nullptr;
	}

	FAssetRuleContext_List Context;
	Context.Index = InRank;

	return RankItems->GetCollectionRule<UAssetCollection>(Context);
}

FPrimaryAssetId UAvatarAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Entity.Avatar"), GetFName());
}

