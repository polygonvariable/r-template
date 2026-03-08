// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDashboardUI.h"

// Engine Headers

// Project Headers
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"



void UInventoryDashboardUI::InitializeDetail()
{
	PrimaryDetail->SetContainerId(ContainerId);
	PrimaryDetail->InitializeDetail();

	PrimaryCollection->SetContainerId(ContainerId);
	PrimaryCollection->InitializeCollection();
	PrimaryCollection->DisplayEntries();
}

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
	Widget->SetContainerId(ContainerId);
	Widget->InitializeDetail();
	Widget->InitializeDetail(SelectedEntry);
}

void UInventoryDashboardUI::GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const
{
	OutAssetUI.Add(PrimaryDetail);
}

void UInventoryDashboardUI::NativeConstruct()
{
	PrimaryCollection->OnEntrySelected.BindUObject(this, &UAssetDashboardUI::InitializeDetail);
	
	Super::NativeConstruct();
}

void UInventoryDashboardUI::NativeDestruct()
{
	PrimaryCollection->OnEntrySelected.Unbind();

	Super::NativeDestruct();
}

