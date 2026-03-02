// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

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
	FPrimaryAssetId TargetAssetId;
	FGameplayTagContainer MaterialTags;

protected:

	int TargetQuantity = 1;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	UPROPERTY()
	TObjectPtr<const UTradeAsset> TradeAsset = nullptr;

	UPROPERTY()
	TObjectPtr<const URPrimaryDataAsset> TargetAsset = nullptr;

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

	void Step_LoadAsset();
	void Step_CheckTarget();
	void Step_CheckMaterial();
	void Step_PerformTransaction(const TMap<FPrimaryAssetId, int>& MaterialAssetList, FPrimaryAssetType MaterialAssetType);

};

