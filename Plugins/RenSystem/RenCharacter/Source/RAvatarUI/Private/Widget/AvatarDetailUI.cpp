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

	UAvatarStorage* AvatarCollection = AvatarSubsystem->GetAvatarCollection(AssetSourceId);
	if (IsValid(AvatarCollection) && bAutoRefresh)
	{
		AvatarCollection->OnRefreshed.AddUObject(this, &UAvatarDetailUI::RefreshDetail);
	}

	AvatarStorage = TWeakObjectPtr<UAvatarStorage>(AvatarCollection);
}

void UAvatarDetailUI::RefreshDetail()
{
	UAvatarStorage* AvatarCollection = AvatarStorage.Get();
	if (!IsValid(AvatarCollection))
	{
		return;
	}

	const FAvatarInstance* Instance = AvatarCollection->GetInstance(GetActiveAssetId());
	if (!Instance)
	{
		return;
	}

	SetCustomDetails(Instance);
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

	const FAvatarInstance* Instance = AvatarEntry->AvatarInstance;
	SetCustomDetails(Instance);
}

void UAvatarDetailUI::SetCustomDetails(const FAvatarInstance* Instance)
{
	if (!Instance)
	{
		return;
	}

	AscensionDetail->InitializeDetail(Instance->Ascension);
}

void UAvatarDetailUI::NativeDestruct()
{
	UAvatarStorage* AvatarCollection = AvatarStorage.Get();
	if (IsValid(AvatarCollection))
	{
		AvatarCollection->OnRefreshed.RemoveAll(this);
	}
	AvatarStorage.Reset();

	Super::NativeDestruct();
}

