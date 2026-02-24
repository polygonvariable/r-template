// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Interface/ShopProviderInterface.h"

// Generated Headers
#include "PurchasableAsset.generated.h"

// Module Macros
#define RSYSTEM_API RINVENTORY_API

// Forward Declarations
class UAssetGroup;
class UAssetCollection;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UPurchasableAsset : public UInventoryAsset, public IShopProviderInterface
{

	GENERATED_BODY()

public:

	// ~ IShopProviderInterface
	RSYSTEM_API virtual const UAssetCollection* GetPurchaseCost() const override;
	// ~ End of IShopProviderInterface

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> PurchaseCost = nullptr;

};



// Module Macros
#undef RSYSTEM_API

