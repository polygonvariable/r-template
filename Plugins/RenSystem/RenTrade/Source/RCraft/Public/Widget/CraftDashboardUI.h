// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/TradeDashboardUI.h"

// Generated Headers
#include "CraftDashboardUI.generated.h"

// Forward Declarations
class UButton;



/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UCraftDashboardUI : public UTradeDashboardUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UButton> CraftButton = nullptr;

	UFUNCTION()
	void HandleCraft();

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

