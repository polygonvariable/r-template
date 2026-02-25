// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/Category/PurchasableAsset.h"
#include "Interface/CraftProviderInterface.h"

// Generated Headers
#include "CraftableAsset.generated.h"

// Module Macros
#define RSYSTEM_API RINVENTORY_API

// Forward Declarations
class UAssetGroup;
class UAssetCollection;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UCraftableAsset : public UPurchasableAsset, public ICraftProviderInterface
{

	GENERATED_BODY()

public:

	// ~ IShopProviderInterface
	RSYSTEM_API virtual const UAssetCollection* GetCraftingItems() const override;
	// ~ End of IShopProviderInterface

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> CraftingItems = nullptr;

};



// Module Macros
#undef RSYSTEM_API

