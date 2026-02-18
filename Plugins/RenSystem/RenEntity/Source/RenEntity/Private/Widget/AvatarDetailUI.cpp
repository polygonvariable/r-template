// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarDetailUI.h"

// Engine Headers
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"

#include "AvatarAsset.h"
#include "EntityPrimaryAsset.h"
#include "Widget/AvatarEntry.h"

#include "Subsystem/AvatarSubsystem.h"



void UAvatarDetailUI::HandleAvatarDataChanged(const FPrimaryAssetId& AssetId)
{
	if (ActiveAssetId == AssetId)
	{
		RefreshDetails();
	}
}

void UAvatarDetailUI::RefreshDetails()
{
	UGameInstance* GameInstance = GetGameInstance();
	UAvatarSubsystem* AvatarSubsystem = GameInstance->GetSubsystem<UAvatarSubsystem>();
	const FAvatarData* Record = AvatarSubsystem->GetAvatarData(ActiveAssetId);
	if (Record)
	{
		SetSecondaryDetails(*Record, nullptr);
	}
}

bool UAvatarDetailUI::IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const
{
	return EntityPrimaryAsset::IsValid(AssetId);
}

void UAvatarDetailUI::SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	const UAvatarAsset* AvatarAsset = Cast<UAvatarAsset>(Asset);
	if (!IsValid(AvatarAsset))
	{
		return;
	}

	if (EntryName) EntryName->SetText(AvatarAsset->DisplayName);
	if (EntryDescription) EntryDescription->SetText(AvatarAsset->Description);
	if (EntryIcon) EntryIcon->SetBrushFromSoftTexture(AvatarAsset->Icon);
}

void UAvatarDetailUI::SetSecondaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	const UAvatarEntry* AvatarEntry = Cast<UAvatarEntry>(Entry);
	if (!IsValid(AvatarEntry))
	{
		return;
	}

	const FAvatarData& Record = AvatarEntry->AvatarData;
	SetSecondaryDetails(Record, Asset);
}

void UAvatarDetailUI::SetSecondaryDetails(const FAvatarData& Record, const UPrimaryDataAsset* Asset)
{
	const FAscensionData& Ascension = Record.Ascension;

	if (EntryRank) EntryRank->SetText(FText::FromString(FString::FromInt(Ascension.Rank)));
	if (EntryLevel) EntryLevel->SetText(FText::FromString(FString::FromInt(Ascension.Level)));
	if (EntryExperience) EntryExperience->SetText(FText::FromString(FString::FromInt(Ascension.Experience)));
}

void UAvatarDetailUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	UAvatarSubsystem* AvatarSubsystem = GameInstance->GetSubsystem<UAvatarSubsystem>();
	AvatarSubsystem->OnAvatarUpdated.AddUObject(this, &UAvatarDetailUI::HandleAvatarDataChanged);

	Super::NativeConstruct();
}

void UAvatarDetailUI::NativeDestruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	UAvatarSubsystem* AvatarSubsystem = GameInstance->GetSubsystem<UAvatarSubsystem>();
	AvatarSubsystem->OnAvatarUpdated.RemoveAll(this);

	Super::NativeDestruct();
}

