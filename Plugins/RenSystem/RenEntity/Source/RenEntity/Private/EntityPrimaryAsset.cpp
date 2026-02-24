// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EntityPrimaryAsset.h"

// Engine Headers

// Project Headers
#include "AvatarAsset.h"


FPrimaryAssetType EntityPrimaryAsset::GetAssetType()
{
	return FPrimaryAssetType(TEXT("Entity.Avatar"));
}

FPrimaryAssetId EntityPrimaryAsset::GetPrimaryAssetId(const FName& AssetName)
{
	return FPrimaryAssetId(EntityPrimaryAsset::GetAssetType(), AssetName);
}

bool EntityPrimaryAsset::IsValid(const FPrimaryAssetId& AssetId)
{
	return AssetId.PrimaryAssetType == EntityPrimaryAsset::GetAssetType();
}

bool EntityPrimaryAsset::GetDisplayName(const FAssetData& AssetData, FText& DisplayName)
{
	return AssetData.GetTagValue<FText>(GET_MEMBER_NAME_CHECKED(UAvatarAsset, DisplayName), DisplayName);
}

bool EntityPrimaryAsset::GetHealth(const FAssetData& AssetData, int& Health)
{
	return AssetData.GetTagValue<int>(GET_MEMBER_NAME_CHECKED(UAvatarAsset, Health), Health);
}


const FName EntityFilterProperty::AssetId(TEXT("AssetId"));
const FName EntityFilterProperty::InfoType(TEXT("InfoType"));
