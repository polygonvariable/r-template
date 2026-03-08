// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarViewUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Asset/AvatarAsset.h"
#include "EntityPrimaryAsset.h"
#include "Filter/FilterLeafCriterion.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Widget/AssetEntry.h"
#include "Widget/AvatarCollectionUI.h"
#include "Widget/AvatarDetailUI.h"



void UAvatarViewUI::HandleEnhanceClicked()
{
	const UAssetEntry* Entry = ActiveEntry.Get();
	const URPrimaryDataAsset* Asset = ActiveAsset.Get();
	if (!IsValid(Entry) || !IsValid(Asset))
	{
		LOG_ERROR(LogAvatar, TEXT("Entry, Asset is invalid"));
		return;
	}

	UAssetUI* Widget = CreateWidget<UAssetUI>(this, EnhanceWidgetClass);
	if (!IsValid(Widget))
	{
		LOG_ERROR(LogAvatar, TEXT("EnhanceWidgetUI is invalid"));
		return;
	}

	Widget->AddToViewport();
	Widget->InitializeDetail(Entry, Asset);
}

void UAvatarViewUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	if (IsValid(AvatarDetail))
	{
		AvatarDetail->InitializeDetail(Entry, Asset);
	}

	ActiveEntry = TWeakObjectPtr<const UAssetEntry>(Entry);
	ActiveAsset = TWeakObjectPtr<const URPrimaryDataAsset>(Asset);
}

void UAvatarViewUI::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UAvatarViewUI::NativeConstruct()
{
	if (IsValid(AvatarCollection))
	{
		AvatarCollection->OnEntrySelected.BindUObject(this, &UAvatarViewUI::InitializeDetail);
	}

	if (IsValid(EnhanceButton))
	{
		EnhanceButton->OnClicked.AddDynamic(this, &UAvatarViewUI::HandleEnhanceClicked);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.AddDynamic(this, &UAvatarViewUI::CloseWidget);
	}

	Super::NativeConstruct();
}

void UAvatarViewUI::NativeDestruct()
{
	if (IsValid(AvatarCollection))
	{
		AvatarCollection->OnEntrySelected.Unbind();
	}

	if (IsValid(EnhanceButton))
	{
		EnhanceButton->OnClicked.RemoveAll(this);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.RemoveAll(this);
	}

	ActiveEntry.Reset();
	ActiveAsset.Reset();

	Super::NativeDestruct();
}

