// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/AvatarAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Storage/AvatarStorage.h"
#include "Subsystem/AvatarSubsystem.h"
#include "Widget/AscensionDetailUI.h"
#include "Widget/AvatarEntry.h"



void UAvatarDetailUI::InitializeDetail()
{
	UAvatarSubsystem* AvatarSubsystem = UAvatarSubsystem::Get(GetGameInstance());
	if (!IsValid(AvatarSubsystem))
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarSubsystem is invalid"));
		return;
	}

	UAvatarStorage* AvatarStorage = AvatarSubsystem->GetAvatarCollection(AssetSourceId);
	if (IsValid(AvatarStorage) && bAutoRefresh)
	{
		AvatarStorage->OnRefreshed.AddUObject(this, &UAvatarDetailUI::RefreshDetail);
	}

	AvatarCollection = TWeakObjectPtr<UAvatarStorage>(AvatarStorage);
}

void UAvatarDetailUI::RefreshDetail()
{
	UAvatarStorage* AvatarStorage = AvatarCollection.Get();
	if (!IsValid(AvatarStorage))
	{
		return;
	}

	const FAvatarData* Item = AvatarStorage->GetItem(GetActiveAssetId());
	if (!Item)
	{
		return;
	}

	SetCustomDetails(Item);
}

void UAvatarDetailUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UAvatarAsset* AvatarAsset = Cast<UAvatarAsset>(Asset);
	if (!IsValid(AvatarAsset))
	{
		SwitchDetail(false);
		return;
	}

	SwitchDetail(true);

	EntryName->SetText(AvatarAsset->DisplayName);
	EntryDescription->SetText(AvatarAsset->Description);
	EntryIcon->SetBrushFromSoftTexture(AvatarAsset->Icon);
}

void UAvatarDetailUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UAvatarEntry* AvatarEntry = Cast<UAvatarEntry>(Entry);
	if (!IsValid(AvatarEntry))
	{
		return;
	}

	const FAvatarData* Item = AvatarEntry->AvatarInstance;
	SetCustomDetails(Item);
}

void UAvatarDetailUI::SetCustomDetails(const FAvatarData* Item)
{
	if (!Item)
	{
		return;
	}

	AscensionDetail->InitializeDetail(Item->Ascension);
}

void UAvatarDetailUI::NativeDestruct()
{
	UAvatarStorage* AvatarStorage = AvatarCollection.Get();
	if (IsValid(AvatarStorage))
	{
		AvatarStorage->OnRefreshed.RemoveAll(this);
	}
	AvatarCollection.Reset();

	Super::NativeDestruct();
}

