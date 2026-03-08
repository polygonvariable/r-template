// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "CraftSubsystem.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(NotBlueprintType)
class UCraftSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void CraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static UCraftSubsystem* Get(UWorld* World);
	static UCraftSubsystem* Get(UGameInstance* GameInstance);

};

