// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "InventoryAscensionSubsystem.generated.h"

// Module Macros
#define RSYSTEM_API RINVENTORYASCENSION_API

// Forward Declarations



/**
 *
 */
UCLASS(NotBlueprintType, MinimalAPI)
class UInventoryAscensionSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	RSYSTEM_API void AddExperiencePoints(FName SourceId, FPrimaryAssetId TargetAssetId, FGuid TargetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId, FTaskCallback Callback);
	RSYSTEM_API void AddRankPoints(FName SourceId, FPrimaryAssetId TargetAssetId, FGuid TargetId, FTaskCallback Callback);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	RSYSTEM_API static UInventoryAscensionSubsystem* Get(UWorld* World);
	RSYSTEM_API static UInventoryAscensionSubsystem* Get(UGameInstance* GameInstance);

};



// Module Macros
#undef RSYSTEM_API

