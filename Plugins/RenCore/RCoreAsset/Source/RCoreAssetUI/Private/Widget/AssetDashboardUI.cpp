// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Widget/AssetEntry.h"



void UAssetDashboardUI::RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass)
{
}

void UAssetDashboardUI::GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const
{

}

void UAssetDashboardUI::SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	TArray<UAssetUI*> AssetUI;
	GetAllAssetUI(AssetUI);

	for (UAssetUI* Catalog : AssetUI)
	{
		if (IsValid(Catalog) && Catalog != this)
		{
			Catalog->InitializeDetails(Entry, Asset);
		}
	}
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

