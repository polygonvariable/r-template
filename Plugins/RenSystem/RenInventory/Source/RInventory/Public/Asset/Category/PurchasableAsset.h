// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "ExchangeRule.h"
#include "ShopProviderInterface.h"
#include "Asset/InventoryAsset.h"

// Generated Headers
#include "PurchasableAsset.generated.h"

// Forward Declarations
class UAssetGroup;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UPurchasableAsset : public UInventoryAsset, public IShopProviderInterface
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FExchangeRule PurchaseRule;

	// ~ IShopProviderInterface
	RINVENTORY_API virtual const UAssetGroup* GetPurchaseGroup() const override
	{
		return nullptr;
	}
	RINVENTORY_API virtual const FExchangeRule& GetPurchaseRule() const override;
	// ~ End of IShopProviderInterface

};

