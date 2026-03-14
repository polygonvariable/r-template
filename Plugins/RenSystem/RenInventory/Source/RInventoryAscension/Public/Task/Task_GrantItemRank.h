// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AscensionData.h"
#include "Task/TaskObject.h"

// Generated Headers
#include "Task_GrantItemRank.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventoryStorage;
class URAssetManager;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UTask_GrantItemRank : public UTaskObject
{

	GENERATED_BODY()

public:

	FName SourceId = NAME_None;
	FGuid TargetId;
	FPrimaryAssetId TargetAssetId;
	
protected:

	UPROPERTY()
	TObjectPtr<const UInventoryAsset> TargetAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UInventoryStorage> Inventory = nullptr;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	int ExperiencePerLevel = 0;
	int LevelPerRank = 0;
	int MaxLevel = 0;
	int MaxRank = 0;

	FAscensionData AscensionData;


	void Step_LoadAsset();
	void Step_CheckTarget();

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

};

