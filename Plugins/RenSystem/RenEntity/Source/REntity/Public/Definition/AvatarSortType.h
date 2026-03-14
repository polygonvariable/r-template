// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/QueryType.h"

// Generated Headers
#include "AvatarSortType.generated.h"

// Forward Declarations
struct FAvatarData;



/**
 *
 * 
 */
UENUM(BlueprintType)
enum class EAvatarSortType : uint8
{
	None UMETA(DisplayName = "None"),
	Alphabetical UMETA(DisplayName = "Alphabetical"),
	Rank UMETA(DisplayName = "Rank"),
	Level UMETA(DisplayName = "Level"),
};



/**
 *
 * 
 */
struct FAvatarSortEntry
{

public:

	FPrimaryAssetId AssetId;
	FText ItemName;
	const FAvatarData* Item = nullptr;

	FAvatarSortEntry(FPrimaryAssetId InAssetId, FText InItemName) : AssetId(InAssetId), ItemName(InItemName) {}
	FAvatarSortEntry(FPrimaryAssetId InAssetId, FText InItemName, const FAvatarData* InItem) : AssetId(InAssetId), ItemName(InItemName), Item(InItem) {}

};

