// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "TaskObject.h"

// Generated Headers
#include "Task_AddAvatarExperience.generated.h"

// Forward Declarations
class UAvatarAsset;
class UAvatarSubsystem;
class URAssetManager;



/**
 *
 *
 *
 */
UCLASS(MinimalAPI, NotBlueprintType)
class UTask_AddAvatarExperience : public UTaskObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId AvatarId;
	FPrimaryAssetId ItemId;

protected:

	UPROPERTY()
	TObjectPtr<UAvatarAsset> AvatarAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UPrimaryDataAsset> ItemAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UAvatarSubsystem> AvatarSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

	void Step_LoadAssets();
	void Step_CheckAssets();
	void Step_RemoveItem();
	void Step_AddExperience();

};

