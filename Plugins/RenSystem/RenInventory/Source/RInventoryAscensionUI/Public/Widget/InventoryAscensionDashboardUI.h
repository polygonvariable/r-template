// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "InventoryAscensionDashboardUI.generated.h"

// Forward Declarations
class UAssetEntry;
class UAssetCollectionUI;
class UAssetDetailUI;
class UInventoryStorage;
class UInventoryAscensionSubsystem;
class URPrimaryDataAsset;
class IAscensionProvider;

struct FInventoryItem;
struct FTaskResult;



/**
 *
 */
UCLASS(Abstract)
class UInventoryAscensionDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDashboardUI
	virtual void InitializeDetail() override;
	virtual void RefreshDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	FGuid ActiveItemId;
	const IAscensionProvider* ActiveItemAscension = nullptr;

	UPROPERTY()
	TObjectPtr<UInventoryAscensionSubsystem> AscensionSubsystem = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UInventoryStorage> Inventory = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> LevelItemCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> RankItemCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> InventoryDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> LevelUpButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> RankUpButton = nullptr;


	void EnableControls();
	void DisableControls();

	void ToggleAscension(const FInventoryItem* Item);
	void ToggleLevelUp(const FInventoryItem* Item);
	void ToggleRankUp(const FInventoryItem* Item);

	void HandleTaskCallback(const FTaskResult& Result);


	UFUNCTION()
	void HandleLevelUp();

	UFUNCTION()
	void HandleRankUp();

	void HandleOnItemUpdated();

	// ~ UAssetDashboardUI
	virtual void SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

