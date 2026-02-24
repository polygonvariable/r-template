// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "ItemAscensionDashboardUI.generated.h"

// Forward Declarations
class UAssetEntry;
class UInventoryCollectionUI;
class UInventoryDetailUI;
class IAscensionProviderInterface;
class UInventorySubsystem;
class UItemAscensionSubsystem;
class URPrimaryDataAsset;

struct FInventoryItem;



/**
 *
 */
UCLASS(Abstract)
class UItemAscensionDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDashboardUI
	virtual void SetCatalogId(const FGuid& Id) override;
	virtual void RefreshDetails() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	FGuid ActiveItemId;
	const IAscensionProviderInterface* ActiveItemAscension = nullptr;

	UPROPERTY()
	TObjectPtr<UItemAscensionSubsystem> AscensionSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UInventorySubsystem> InventorySubsystem = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> PrimaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UInventoryDetailUI> PrimaryDetail = nullptr;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UButton> LevelUpButton = nullptr;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UButton> RankUpButton = nullptr;


	void ToggleAscension(const FInventoryItem* Item);
	void ToggleLevelUp();
	void ToggleRankUp();


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

