// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "TaskDefinition.h"

// Generated Headers
#include "AvatarAscensionSubsystem.generated.h"

// Forward Declarations
class UTaskSubsystem;
class UTaskObject;



/**
 *
 *
 *
 */
UCLASS(MinimalAPI, NotBlueprintType)
class UAvatarAscensionSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void AddExperiencePoints(FGuid TaskId, const FPrimaryAssetId& AvatarId, const FPrimaryAssetId& ItemId, FTaskCallback Callback);
	void AddRankPoints(FGuid TaskId, const FPrimaryAssetId& AvatarId);

protected:

	UPROPERTY()
	TObjectPtr<UTaskSubsystem> TaskSubsystem = nullptr;

	void OnGameLoaded();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};

