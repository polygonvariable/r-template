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

#include "Asset/AvatarAsset.h"
#include "EntityPrimaryAsset.h"
#include "Widget/AvatarEntry.h"

#include "Subsystem/AvatarSubsystem.h"



void UAvatarDetailUI::HandleAvatarDataChanged(const FPrimaryAssetId& AssetId)
{
	if (GetActiveAssetId() == AssetId)
	{
		RefreshDetail();
	}
}

void UAvatarDetailUI::RefreshDetail()
{
	UGameInstance* GameInstance = GetGameInstance();
	UAvatarSubsystem* AvatarSubsystem = GameInstance->GetSubsystem<UAvatarSubsystem>();
	const FAvatarData* Record = AvatarSubsystem->GetAvatarData(GetActiveAssetId());
	if (Record)
	{
		SetSecondaryDetail(*Record, nullptr);
	}
}

void UAvatarDetailUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
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

void UAvatarDetailUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UAvatarEntry* AvatarEntry = Cast<UAvatarEntry>(Entry);
	if (!IsValid(AvatarEntry))
	{
		return;
	}

	const FAvatarData& Record = AvatarEntry->AvatarData;
	SetSecondaryDetail(Record, Asset);
}

void UAvatarDetailUI::SetSecondaryDetail(const FAvatarData& Record, const URPrimaryDataAsset* Asset)
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

