// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "InventoryContainer.h"

// Generated Headers
#include "InventoryStorage.generated.h"



/**
 *
 */
UCLASS()
class UInventoryStorage : public USaveGame
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FPrimaryAssetId, FInventoryStack> InventoryStacks;

};

