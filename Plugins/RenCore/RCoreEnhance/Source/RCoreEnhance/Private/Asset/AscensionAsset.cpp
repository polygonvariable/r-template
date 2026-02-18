// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/AscensionAsset.h"

// Engine Headers

// Project Headers




FPrimaryAssetId UMetadataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Metadata"), GetFName());
}

int UExperiencePointAsset::GetPoints(int Quality) const
{
	return Points * Quality;
}

int URankPointAsset::GetPoints(int Quality) const
{
	return Points * Quality;
}


