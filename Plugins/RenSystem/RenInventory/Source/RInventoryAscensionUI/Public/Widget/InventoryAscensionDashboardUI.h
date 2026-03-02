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
class UInventorySubsystem;
class UInventoryAscensionSubsystem;
class URPrimaryDataAsset;
class IAscensionProviderInterface;

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
	virtual void SetContainerId(const FGuid& Id) override;
	virtual void RefreshDetails() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	FGuid ActiveItemId;
	const IAscensionProviderInterface* ActiveItemAscension = nullptr;

	UPROPERTY()
	TObjectPtr<UInventoryAscensionSubsystem> AscensionSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UInventorySubsystem> InventorySubsystem = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> PrimaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> SecondaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> PrimaryDetail = nullptr;

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

	void HandleOnItemUpdated(const FGuid& InventoryId);

	// ~ UAssetDashboardUI
	virtual void SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	virtual void GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

