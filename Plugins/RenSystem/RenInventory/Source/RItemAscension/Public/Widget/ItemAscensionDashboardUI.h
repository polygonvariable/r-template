// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogDashboardUI.h"

// Generated Headers
#include "ItemAscensionDashboardUI.generated.h"

// Forward Declarations
class UCatalogEntry;
class UInventoryCollectionUI;
class UInventoryDetailUI;
class IAscensionProviderInterface;
class UInventorySubsystem;
class UItemAscensionSubsystem;

struct FInventoryItem;



/**
 *
 */
UCLASS(Abstract)
class UItemAscensionDashboardUI : public UCatalogDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UCatalogDashboardUI
	virtual void SetCatalogId(const FGuid& Id) override;
	virtual void RefreshDetails() override;
	// ~ End of UCatalogDashboardUI

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

	// ~ UCatalogDashboardUI
	virtual void SetSecondaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	virtual void GetAllCatalogUI_Implementation(TArray<UCatalogUI*>& OutCatalogUI) const;
	// ~ End of UCatalogDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

