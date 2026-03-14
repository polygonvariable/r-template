// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/EntityPrimaryAsset.h"

// Project Headers
#include "Asset/AvatarAsset.h"



FPrimaryAssetType FEntityPrimaryAsset::GetAssetType()
{
	return UAvatarAsset::GetPrimaryAssetType();
}

FPrimaryAssetId FEntityPrimaryAsset::GetPrimaryAssetId(const FName& AssetName)
{
	return FPrimaryAssetId(FEntityPrimaryAsset::GetAssetType(), AssetName);
}

bool FEntityPrimaryAsset::IsValid(const FPrimaryAssetId& AssetId)
{
	return AssetId.PrimaryAssetType == FEntityPrimaryAsset::GetAssetType();
}

bool FEntityPrimaryAsset::GetDisplayName(const FAssetData& AssetData, FText& DisplayName)
{
	if (!AssetData.IsValid())
	{
		return false;
	}
	return AssetData.GetTagValue<FText>(GET_MEMBER_NAME_CHECKED(UAvatarAsset, DisplayName), DisplayName);
}

bool FEntityPrimaryAsset::GetHealth(const FAssetData& AssetData, int& Health)
{
	if (!AssetData.IsValid())
	{
		return false;
	}
	return AssetData.GetTagValue<int>(GET_MEMBER_NAME_CHECKED(UAvatarAsset, Health), Health);
}

