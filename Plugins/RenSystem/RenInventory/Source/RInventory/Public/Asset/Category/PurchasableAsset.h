// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "ExchangeRule.h"
#include "ShopProviderInterface.h"
#include "InventoryAsset.h"

// Generated Headers
#include "PurchasableAsset.generated.h"

// Forward Declarations
class UAssetCollectionGroup;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UPurchasableAsset : public UInventoryAsset, public IShopProviderInterface
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetCollectionGroup> PurchaseGroup = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FExchangeRule PurchaseRule;

	// ~ IShopProviderInterface
	RINVENTORY_API virtual const UAssetCollectionGroup* GetPurchaseGroup() const override
	{
		return PurchaseGroup;
	}
	RINVENTORY_API virtual const FExchangeRule& GetPurchaseRule() const override;
	// ~ End of IShopProviderInterface

};

