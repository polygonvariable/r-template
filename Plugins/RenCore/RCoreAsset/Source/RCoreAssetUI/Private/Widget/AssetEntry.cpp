// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetEntry.h"

// Engine Headers

// Project Headers



FPrimaryAssetId UAssetEntry::GetAssetId() const
{
	return FPrimaryAssetId();
}

FGuid UAssetEntry::GetAssetInstanceId() const
{
	return FGuid();
}

void UAssetEntry::ResetData()
{
	AssetId = FPrimaryAssetId();
	AssetSubDetail.Reset();
}

void UAssetEntry::BeginDestroy()
{
	ResetData();
	Super::BeginDestroy();
}

