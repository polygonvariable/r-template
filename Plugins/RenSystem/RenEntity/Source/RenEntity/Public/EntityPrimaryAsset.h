// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers

// Forward Declarations



/**
 *
 */
class EntityPrimaryAsset
{

public:

	static FPrimaryAssetType GetAssetType();
	static FPrimaryAssetId GetPrimaryAssetId(const FName& AssetName);

	static bool IsValid(const FPrimaryAssetId& AssetId);

	static bool GetDisplayName(const FAssetData& AssetData, FText& DisplayName);
	static bool GetInfoType(const FAssetData& AssetData, FName& InfoType);
	static bool GetHealth(const FAssetData& AssetData, int& Health);

};


/**
 *
 */
class EntityFilterProperty
{

public:

	static const FName AssetId;
	static const FName InfoType;

};

