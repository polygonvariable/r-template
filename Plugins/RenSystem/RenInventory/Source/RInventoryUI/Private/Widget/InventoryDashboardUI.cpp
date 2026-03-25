// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDashboardUI.h"

// Engine Headers

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"



void UInventoryDashboardUI::InitializeDetail()
{
	InventoryDetail->PrimarySourceId = PrimarySourceId;
	InventoryDetail->InitializeDetail();

	InventoryCollection->PrimarySourceId = PrimarySourceId;
	InventoryCollection->InitializeCollection();
	InventoryCollection->DisplayEntries();
}

void UInventoryDashboardUI::InitializeAssetDetail(const URPrimaryDataAsset* Asset)
{
	UAssetEntry* Entry = InventoryCollection->GetSelectedEntry();

	InventoryDetail->InitializeEntryDetail(Entry);
	InventoryDetail->InitializeAssetDetail(Asset);
}

void UInventoryDashboardUI::ResetDetail()
{
	InventoryDetail->ResetDetail();
}

void UInventoryDashboardUI::RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass)
{
	const UAssetEntry* SelectedEntry = InventoryCollection->GetSelectedEntry();
	if (!IsValid(WidgetClass) || !IsValid(SelectedEntry))
	{
		return;
	}

	UAssetDashboardUI* Widget = CreateWidget<UAssetDashboardUI>(this, WidgetClass);
	if (!IsValid(Widget))
	{
		return;
	}

	Widget->PrimarySourceId = PrimarySourceId;
	Widget->AddToViewport();
	Widget->InitializeDetail();
	Widget->InitializeAssetByEntry(SelectedEntry);
}

void UInventoryDashboardUI::NativeConstruct()
{
	InventoryCollection->OnSelectionChanged.BindUObject(this, &UAssetDashboardUI::InitializeAssetByEntry);
	InventoryCollection->OnSelectionCleared.BindUObject(this, &UAssetDashboardUI::ResetDetail);
	
	Super::NativeConstruct();
}

void UInventoryDashboardUI::NativeDestruct()
{
	InventoryCollection->OnSelectionChanged.Unbind();
	InventoryCollection->OnSelectionCleared.Unbind();

	Super::NativeDestruct();
}

