// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AvatarAsset.h"

// Engine Headers

// Project Headers



const UAssetCollectionRule_Dictionary* UAvatarAsset::GetExperienceItems(int InExperience, int InLevel, int InRank) const
{
	if (!IsValid(ExperienceItems))
	{
		return nullptr;
	}

	return ExperienceItems->GetCollectionRule<UAssetCollectionRule_Dictionary>();
}


const UAssetCollectionRule_Dictionary* UAvatarAsset::GetRankItems(int InExperience, int InLevel, int InRank) const
{
	if (!IsValid(RankItems))
	{
		return nullptr;
	}

	FAssetRuleContext_Sequence Context;
	Context.Index = InRank;

	return RankItems->GetCollectionRule<UAssetCollectionRule_Dictionary>(Context);
}

FPrimaryAssetId UAvatarAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Entity.Avatar"), GetFName());
}

