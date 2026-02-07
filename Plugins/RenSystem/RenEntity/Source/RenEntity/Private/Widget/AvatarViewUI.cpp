// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarViewUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "AvatarAsset.h"
#include "FilterLeafCriterion.h"
#include "LogCategory.h"
#include "LogMacro.h"
#include "Widget/CatalogEntry.h"

#include "EntityPrimaryAsset.h"
#include "Widget/AvatarCollectionUI.h"
#include "Widget/AvatarDetailUI.h"



void UAvatarViewUI::HandleEnhanceClicked()
{
	const UCatalogEntry* Entry = ActiveEntry.Get();
	const UPrimaryDataAsset* Asset = ActiveAsset.Get();
	if (!IsValid(Entry) || !IsValid(Asset))
	{
		LOG_ERROR(LogAvatar, TEXT("Entry, Asset is invalid"));
		return;
	}

	UCatalogUI* Widget = CreateWidget<UCatalogUI>(this, EnhanceWidgetClass);
	if (!IsValid(Widget))
	{
		LOG_ERROR(LogAvatar, TEXT("EnhanceWidgetUI is invalid"));
		return;
	}

	Widget->AddToViewport();
	Widget->InitializeDetails(Entry, Asset);
}

void UAvatarViewUI::SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	if (IsValid(AvatarDetail))
	{
		AvatarDetail->InitializeDetails(Entry, Asset);
	}

	ActiveEntry = TWeakObjectPtr<const UCatalogEntry>(Entry);
	ActiveAsset = TWeakObjectPtr<const UPrimaryDataAsset>(Asset);
}

void UAvatarViewUI::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UAvatarViewUI::NativeConstruct()
{
	if (IsValid(AvatarCollection))
	{
		AvatarCollection->OnEntrySelected.BindUObject(this, &UAvatarViewUI::InitializeDetails);
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

