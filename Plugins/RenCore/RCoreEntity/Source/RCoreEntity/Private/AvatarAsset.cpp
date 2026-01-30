// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AvatarAsset.h"

// Engine Headers

// Project Headers



FPrimaryAssetId UAvatarAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UAvatarAsset::GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType UAvatarAsset::GetPrimaryAssetType()
{
	return TEXT("Entity.Avatar");
}

FPrimaryAssetId UAvatarAsset::MakePrimaryAssetId(const FName& AssetName)
{
	return FPrimaryAssetId(UAvatarAsset::GetPrimaryAssetType(), AssetName);
}

