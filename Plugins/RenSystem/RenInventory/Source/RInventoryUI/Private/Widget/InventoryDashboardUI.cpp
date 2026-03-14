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
	InventoryDetail->AssetSourceId = AssetSourceId;
	InventoryDetail->InitializeDetail();

	InventoryCollection->AssetSourceId = AssetSourceId;
	InventoryCollection->InitializeCollection();
	InventoryCollection->DisplayEntries();
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

	Widget->AssetSourceId = AssetSourceId;
	Widget->AddToViewport();
	Widget->InitializeDetail();
	Widget->InitializeDetail(SelectedEntry);
}

void UInventoryDashboardUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	InventoryDetail->InitializeDetail(Entry, Asset);
}

void UInventoryDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(Asset);
	if (!IsValid(InventoryAsset))
	{
		return;
	}
	SetCustomDetail(InventoryAsset);
}

void UInventoryDashboardUI::NativeConstruct()
{
	InventoryCollection->OnSelectionChanged.BindUObject(this, &UAssetDashboardUI::InitializeDetail);
	InventoryCollection->OnSelectionCleared.BindUObject(this, &UAssetDashboardUI::ResetDetail);
	
	Super::NativeConstruct();
}

void UInventoryDashboardUI::NativeDestruct()
{
	InventoryCollection->OnSelectionChanged.Unbind();
	InventoryCollection->OnSelectionCleared.Unbind();

	Super::NativeDestruct();
}

