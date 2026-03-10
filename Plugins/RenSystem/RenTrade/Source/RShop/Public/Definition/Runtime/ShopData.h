// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "ShopData.generated.h"

// Module Macros
#define RSYSTEM_API RSHOP_API

// Forward Declarations



/**
 *
 */
USTRUCT(BlueprintType)
struct FShopData
{

	GENERATED_BODY()

public:

	FShopData() {}
	FShopData(int InPurchaseCount);


	UPROPERTY(SaveGame)
	int PurchaseCount;


	RSYSTEM_API bool IsValid() const;
	RSYSTEM_API void Reset();
	RSYSTEM_API void Sanitize();

};



// Module Macros
#undef RSYSTEM_API

