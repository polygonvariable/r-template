// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "SaveGame/Storage.h"

// Project Headers

// Generated Headers
#include "ShopStorage.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct FShopStack
{

	GENERATED_BODY()

public:
	
	UPROPERTY(SaveGame)
	FGuid ItemID;

	UPROPERTY(SaveGame)
	int Quantity;

};



/**
 *
 */
UCLASS()
class UItemShopStorage : public UStorage
{

	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TMap<FGuid, FShopStack> ShopItems;

};

