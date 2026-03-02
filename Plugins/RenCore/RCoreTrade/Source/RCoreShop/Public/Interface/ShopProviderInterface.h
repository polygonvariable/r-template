// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "ShopProviderInterface.generated.h"

// Module Macros
#define RCORE_API RCORESHOP_API

// Forward Declarations
class UAssetCollection;

struct FInstancedStruct;



UINTERFACE(MinimalAPI)
class UShopProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORE_API IShopProviderInterface
{

	GENERATED_BODY()

public:

	virtual const UAssetCollection* GetPurchaseCost() const = 0;
	virtual const UAssetCollection* GetPurchaseCost(const FInstancedStruct& Context) const = 0;

};



// Module Macros
#undef RCORE_API

