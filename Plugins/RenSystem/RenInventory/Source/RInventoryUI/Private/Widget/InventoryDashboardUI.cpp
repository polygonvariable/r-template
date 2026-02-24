// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDashboardUI.h"

// Engine Headers

// Project Headers
#include "Widget/AssetEntry.h"
#include "Widget/InventoryCollectionUI.h"
#include "Widget/InventoryDetailUI.h"



void UInventoryDashboardUI::RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass)
{
	const UAssetEntry* SelectedEntry = PrimaryCollection->GetSelectedEntry();
	if (!IsValid(WidgetClass) || !IsValid(SelectedEntry))
	{
		return;
	}

	UAssetDashboardUI* Widget = CreateWidget<UAssetDashboardUI>(this, WidgetClass);
	if (!IsValid(Widget))
	{
		return;
	}
	
	Widget->AddToViewport();
	Widget->SetCatalogId(CatalogId);
	Widget->InitializeDetails(SelectedEntry);
}

void UInventoryDashboardUI::GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const
{
	OutAssetUI.Add(PrimaryDetail);
}

void UInventoryDashboardUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	PrimaryCollection->SetCatalogId(CatalogId);
	PrimaryDetail->SetCatalogId(CatalogId);
}

void UInventoryDashboardUI::NativeConstruct()
{
	PrimaryCollection->OnEntrySelected.BindUObject(this, &UInventoryDashboardUI::InitializeDetails);
	PrimaryCollection->DisplayEntries();
	
	Super::NativeConstruct();
}

void UInventoryDashboardUI::NativeDestruct()
{
	PrimaryCollection->OnEntrySelected.Unbind();

	Super::NativeDestruct();
}

