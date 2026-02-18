// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/InventoryStack.h"
#include "SaveGame/Storage.h"

// Generated Headers
#include "InventoryStorage.generated.h"



/**
 *
 */
UCLASS()
class UInventoryStorage : public UStorage
{

	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, FInventoryStack> InventoryStacks;

};

