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
UCLASS(Abstract)
class UInventoryDashboardUI : public UCatalogDashboardUI
{

	GENERATED_BODY()

public:

	void SetContainerId(FGuid Id);

protected:

	UPROPERTY(EditAnywhere)
	FGuid ContainerId;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> CatalogCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UInventoryDetailUI> CatalogDetail = nullptr;


	// ~ UCatalogDashboardUI
	virtual void GetAllCatalogUI_Implementation(TArray<UCatalogUI*>& OutComponent) const;
	// ~ End of UCatalogDashboardUI

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

