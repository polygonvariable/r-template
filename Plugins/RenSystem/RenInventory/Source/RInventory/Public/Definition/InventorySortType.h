// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/QueryType.h"

// Generated Headers
#include "InventorySortType.generated.h"

// Forward Declarations
struct FInventoryItem;



/**
 *
 * 
 */
UENUM(BlueprintType)
enum class EInventorySortType : uint8
{
	None UMETA(DisplayName = "None"),
	Alphabetical UMETA(DisplayName = "Alphabetical"),
	Quantity UMETA(DisplayName = "Quantity"),
	Rank UMETA(DisplayName = "Rank"),
	Level UMETA(DisplayName = "Level"),
};



/**
 *
 * 
 */
struct FInventorySortEntry
{

public:

	FPrimaryAssetId AssetId;
	FText ItemName;
	int Quantity = 0;
	const FInventoryItem* Item = nullptr;

	FInventorySortEntry(FPrimaryAssetId InAssetId, FText InItemName, int InQuantity) : AssetId(InAssetId), ItemName(InItemName), Quantity(InQuantity) {}
	FInventorySortEntry(FPrimaryAssetId InAssetId, FText InItemName, int InQuantity, const FInventoryItem* InItem) : AssetId(InAssetId), ItemName(InItemName), Quantity(InQuantity), Item(InItem) {}

};

