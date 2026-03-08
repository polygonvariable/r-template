// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "InventoryDashboardUI.generated.h"

// Forward Declarations
class UAssetCollectionUI;
class UAssetDetailUI;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDashboardUI
	virtual void InitializeDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> PrimaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> PrimaryDetail = nullptr;


	// ~ UAssetDashboardUI
	virtual void RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass) override;
	virtual void GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

