// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDashboardUI.h"

// Engine Headers

// Project Headers
#include "Widget/CatalogEntry.h"
#include "Widget/InventoryCollectionUI.h"
#include "Widget/InventoryDetailUI.h"



void UInventoryDashboardUI::RedirectToWidget(TSubclassOf<UCatalogDashboardUI> WidgetClass)
{
	const UCatalogEntry* SelectedEntry = PrimaryCollection->GetSelectedEntry();
	if (!IsValid(WidgetClass) || !IsValid(SelectedEntry))
	{
		return;
	}

	UCatalogDashboardUI* Widget = CreateWidget<UCatalogDashboardUI>(this, WidgetClass);
	if (!IsValid(Widget))
	{
		return;
	}
	
	Widget->AddToViewport();
	Widget->SetCatalogId(CatalogId);
	Widget->InitializeDetails(SelectedEntry);
}

void UInventoryDashboardUI::GetAllCatalogUI_Implementation(TArray<UCatalogUI*>& OutCatalogUI) const
{
	OutCatalogUI.Add(PrimaryDetail);
}

void UInventoryDashboardUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(PrimaryCollection)) PrimaryCollection->SetCatalogId(CatalogId);
	if (IsValid(PrimaryDetail)) PrimaryDetail->SetCatalogId(CatalogId);
}

void UInventoryDashboardUI::NativeConstruct()
{
	if (IsValid(PrimaryCollection)) PrimaryCollection->OnEntrySelected.BindUObject(this, &UInventoryDashboardUI::InitializeDetails);
	
	Super::NativeConstruct();
}

void UInventoryDashboardUI::NativeDestruct()
{
	if (IsValid(PrimaryCollection)) PrimaryCollection->OnEntrySelected.Unbind();

	Super::NativeDestruct();
}

