// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "ShopSubsystem.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS()
class UShopSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void PurchaseItem(const FGuid& TaskId, const FPrimaryAssetId& ShopAssetId, const FPrimaryAssetId& ItemAssetId, const FPrimaryAssetId& CostAssetId, FTaskCallback Callback);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static UShopSubsystem* Get(UWorld* World);
	static UShopSubsystem* Get(UGameInstance* GameInstance);

};

