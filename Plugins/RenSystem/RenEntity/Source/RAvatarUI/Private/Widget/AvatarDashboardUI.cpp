// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarDashboardUI.h"

// Engine Headers

// Project Headers
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"



void UAvatarDashboardUI::InitializeDetail()
{
	AvatarDetail->AssetSourceId = AssetSourceId;
	AvatarDetail->InitializeDetail();

	AvatarCollection->AssetSourceId = AssetSourceId;
	AvatarCollection->InitializeCollection();
	AvatarCollection->DisplayEntries();
}

void UAvatarDashboardUI::ResetDetail()
{
	AvatarDetail->ResetDetail();
}

void UAvatarDashboardUI::RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass)
{
	const UAssetEntry* SelectedEntry = AvatarCollection->GetSelectedEntry();
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

void UAvatarDashboardUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	AvatarDetail->InitializeDetail(Entry, Asset);
}

void UAvatarDashboardUI::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UAvatarDashboardUI::NativeConstruct()
{
	AvatarCollection->OnSelectionChanged.BindUObject(this, &UAssetDashboardUI::InitializeDetail);
	AvatarCollection->OnSelectionCleared.BindUObject(this, &UAssetDashboardUI::ResetDetail);

	Super::NativeConstruct();
}

void UAvatarDashboardUI::NativeDestruct()
{
	AvatarCollection->OnSelectionChanged.Unbind();
	AvatarCollection->OnSelectionCleared.Unbind();

	Super::NativeDestruct();
}

