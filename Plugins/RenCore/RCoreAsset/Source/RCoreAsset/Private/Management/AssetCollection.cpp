// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Management/AssetCollection.h"

// Engine Headers

// Project Headers



const FGameplayTagContainer& UAssetCollection::GetCollectionTags() const
{
	return Tags;
}

const FPrimaryAssetType& UAssetCollection::GetCollectionType() const
{
	return AssetType;
}

bool UAssetCollection::GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const
{
	return false;
}

bool UAssetCollection::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const
{
	return false;
}

void UAssetCollection::GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const
{

}

void UAssetCollection::GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const
{

}

void UAssetCollection::GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const
{

}

