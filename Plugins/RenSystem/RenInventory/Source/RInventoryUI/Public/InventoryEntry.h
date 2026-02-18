// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogEntry.h"

// Generated Headers
#include "InventoryEntry.generated.h"

// Forward Declarations
struct FInventoryItem;



/**
 *
 */
UCLASS(MinimalAPI)
class UInventoryEntry : public UCatalogEntry
{

	GENERATED_BODY()

public:

	int Quantity = 0;
	const FInventoryItem* Item = nullptr;

	// ~ UCatalogEntry
	virtual void ResetData() override
	{
		Super::ResetData();

		Quantity = 0;
		Item = nullptr;
	}
	// ~ End of UCatalogEntry

};

