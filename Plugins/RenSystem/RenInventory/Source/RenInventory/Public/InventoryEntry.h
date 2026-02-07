// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Project Headers
#include "Widget/CatalogEntry.h"

// Generated Headers
#include "InventoryEntry.generated.h"

// Forward Declarations
struct FInventoryRecord;



/**
 *
 */
UCLASS()
class UInventoryEntry : public UCatalogEntry
{

	GENERATED_BODY()

public:

	int Quantity = 0;

	const FInventoryRecord* Record = nullptr;

	bool bHasPayload = false;

	FInstancedStruct Payload;

	virtual void ResetData() override
	{
		Super::ResetData();

		Quantity = 0;
		Record = nullptr;

		bHasPayload = false;
		Payload.Reset();
	}

	// ~ UObject
	virtual void BeginDestroy() override
	{
		ResetData();
		Super::BeginDestroy();
	}
	// ~ End of UObject

};

