// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "ItemAscensionSubsystem.generated.h"

// Forward Declarations
class UTaskSubsystem;



/**
 *
 */
UCLASS()
class UItemAscensionSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void AddExperiencePoints(FGuid TaskId, const FGuid& InventoryId, const FPrimaryAssetId& TargetAssetId, const FGuid& TargetId, const FPrimaryAssetId& MaterialAssetId, const FGuid& MaterialId, FTaskCallback Callback);
	void AddRankPoints(FGuid TaskId, const FGuid& InventoryId, const FPrimaryAssetId& TargetAssetId, const FGuid& TargetId, FTaskCallback Callback);

protected:

	UPROPERTY()
	TObjectPtr<UTaskSubsystem> TaskSubsystem = nullptr;


	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static UItemAscensionSubsystem* Get(UWorld* World);
	static UItemAscensionSubsystem* Get(UGameInstance* GameInstance);

};

