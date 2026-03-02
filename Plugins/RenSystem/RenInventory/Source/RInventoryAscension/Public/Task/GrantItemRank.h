// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AscensionData.h"
#include "Task/TaskObject.h"

// Generated Headers
#include "GrantItemRank.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventorySubsystem;
class URAssetManager;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UGrantItemRank : public UTaskObject
{

	GENERATED_BODY()

public:

	FGuid InventoryId;

	FGuid TargetId;

	FPrimaryAssetId TargetAssetId;
	
protected:

	UPROPERTY()
	TObjectPtr<const UInventoryAsset> TargetAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UInventorySubsystem> InventorySubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

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

	void Step_LoadAsset();
	void Step_CheckTarget();

};

