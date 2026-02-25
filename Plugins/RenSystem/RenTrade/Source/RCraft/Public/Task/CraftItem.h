// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Task/TaskObject.h"

// Generated Headers
#include "CraftItem.generated.h"

// Forward Declarations
class URAssetManager;
class UTradeAsset;
class URPrimaryDataAsset;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UCraftItem : public UTaskObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId CraftAssetId;
	FPrimaryAssetId ItemAssetId;
	FPrimaryAssetId CostAssetId;

protected:

	FGuid ItemId;
	int ItemQuantity = 1;
	FGuid CostId;
	int CostQuantity = 1;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	UPROPERTY()
	TObjectPtr<const UTradeAsset> ShopAsset = nullptr;

	UPROPERTY()
	TObjectPtr<const URPrimaryDataAsset> ItemAsset = nullptr;

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

	void Step_LoadShopAsset();
	void Step_CheckItem();
	void Step_CheckCost();
	void Step_CraftItem();

};

