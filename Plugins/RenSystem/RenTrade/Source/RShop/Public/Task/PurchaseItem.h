// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Task/TaskObject.h"

// Generated Headers
#include "PurchaseItem.generated.h"

// Forward Declarations
class URAssetManager;
class UTradeAsset;
class URPrimaryDataAsset;
class UAssetCollection;

struct FAssetDetail;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UPurchaseItem : public UTaskObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId ShopAssetId;
	FPrimaryAssetId TargetAssetId;
	FGameplayTagContainer CostTags;

protected:

	int TargetQuantity = 1;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	UPROPERTY()
	TObjectPtr<const UTradeAsset> ShopAsset = nullptr;

	UPROPERTY()
	TObjectPtr<const URPrimaryDataAsset> TargetAsset = nullptr;


	void Step_LoadAsset();
	void Step_CheckTarget();
	void Step_CheckCost();
	void Step_PerformTransaction(const TMap<FPrimaryAssetId, int>& CostAssetList, FPrimaryAssetType CostAssetType);

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

};

