// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers



void UAssetDashboardUI::RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass)
{

}

void UAssetDashboardUI::NativeConstruct()
{
	if (IsValid(CloseButton)) CloseButton->OnClicked.AddDynamic(this, &UAssetDashboardUI::CloseWidget);

	Super::NativeConstruct();
}

void UAssetDashboardUI::NativeDestruct()
{
	if (IsValid(CloseButton)) CloseButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

