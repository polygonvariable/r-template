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
struct FInstancedStruct;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UCraftableAsset : public UPurchasableAsset, public ICraftProviderInterface
{

	GENERATED_BODY()

public:

	// ~ ICraftProviderInterface
	RSYSTEM_API virtual FTimespan GetCraftingTime() const override;
	RSYSTEM_API virtual const UAssetCollection* GetCraftingMaterial() const override;
	RSYSTEM_API virtual const UAssetCollection* GetCraftingMaterial(const FInstancedStruct& Context) const override;
	// ~ End of ICraftProviderInterface

protected:

	UPROPERTY(EditDefaultsOnly)
	FTimespan CraftingTime;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> CraftingItems = nullptr;

};



// Module Macros
#undef RSYSTEM_API

