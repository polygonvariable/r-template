// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AscensionData.h"
#include "Task/TaskObject.h"

// Generated Headers
#include "GrantItemExperience.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventorySubsystem;
class URAssetManager;
class URPrimaryDataAsset;
struct FGameplayTagContainer;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UGrantItemExperience : public UTaskObject
{

	GENERATED_BODY()

public:

	FGuid InventoryId;

	FGuid TargetId;
	FGuid MaterialId;

	FPrimaryAssetId TargetAssetId;
	FPrimaryAssetId MaterialAssetId;
	
protected:

	UPROPERTY()
	TObjectPtr<const UInventoryAsset> TargetAsset = nullptr;

	UPROPERTY()
	TObjectPtr<const URPrimaryDataAsset> MaterialAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UInventorySubsystem> InventorySubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	int MaterialQuantity = 0;
	int Points = 0;

	int ExperiencePerLevel = 0;
	int LevelPerRank = 0;
	int MaxLevel = 0;
	int MaxRank = 0;

	FAscensionData AscensionData;

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

	void Step_LoadAssets();
	void Step_CheckItemAsset();
	void Step_CheckMaterialAsset(const FGameplayTagContainer& RuleTags);
	void Step_LoadBreakdownAsset(const FPrimaryAssetId& AssetId, int Quantity);
	void Step_RemoveItem();
	void Step_AddExperience();

};

