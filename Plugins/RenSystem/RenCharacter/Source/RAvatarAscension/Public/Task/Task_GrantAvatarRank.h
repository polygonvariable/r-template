// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AscensionData.h"
#include "Task/TaskObject.h"

// Generated Headers
#include "Task_GrantAvatarRank.generated.h"

// Forward Declarations
class UAvatarAsset;
class UAvatarStorage;
class URAssetManager;
class IAscensionProvider;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UTask_GrantAvatarRank : public UTaskObject
{

	GENERATED_BODY()

public:

	FName TargetSourceId = NAME_None;
	FName MaterialSourceId = NAME_None;

	FPrimaryAssetId TargetAssetId;
	
protected:

	UPROPERTY()
	TObjectPtr<const UAvatarAsset> TargetAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UAvatarStorage> AvatarStorage = nullptr;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	int ExperiencePerLevel = 0;
	int LevelPerRank = 0;
	int MaxLevel = 0;
	int MaxRank = 0;

	FAscensionData AscensionData;


	void Step_LoadAsset();
	void Step_CheckTarget();
	void Step_RemoveMaterial(const TMap<FPrimaryAssetId, int>& Materials, FPrimaryAssetType MaterialType);
	void Step_AddRank();

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

};

