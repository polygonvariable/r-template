// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogDashboardUI.h"

// Generated Headers
#include "InventoryDashboardUI.generated.h"

// Forward Declarations
class UInventoryCollectionUI;
class UInventoryDetailUI;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryDashboardUI : public UCatalogDashboardUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> PrimaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UInventoryDetailUI> PrimaryDetail = nullptr;

	UFUNCTION(BlueprintCallable)
	void RedirectToWidget(TSubclassOf<UCatalogDashboardUI> WidgetClass);


	// ~ UCatalogDashboardUI
	virtual void GetAllCatalogUI_Implementation(TArray<UCatalogUI*>& OutCatalogUI) const;
	// ~ End of UCatalogDashboardUI

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

