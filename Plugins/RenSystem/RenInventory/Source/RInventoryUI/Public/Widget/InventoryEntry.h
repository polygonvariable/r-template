// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetEntry.h"

// Generated Headers
#include "InventoryEntry.generated.h"

// Forward Declarations
struct FInventoryItem;



/**
 *
 */
UCLASS(MinimalAPI)
class UInventoryEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	int Quantity = 0;
	const FInventoryItem* Item = nullptr;

	// ~ UAssetEntry
	virtual void ResetData() override
	{
		Super::ResetData();

		Quantity = 0;
		Item = nullptr;
	}
	// ~ End of UAssetEntry

};

