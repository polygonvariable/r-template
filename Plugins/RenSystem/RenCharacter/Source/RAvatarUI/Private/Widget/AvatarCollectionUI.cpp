// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarCollectionUI.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Storage/AvatarStorage.h"
#include "Subsystem/AvatarSubsystem.h"
#include "Widget/AvatarEntry.h"



void UAvatarCollectionUI::InitializeCollection()
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
		AvatarCollection->OnRefreshed.AddUObject(this, &UAvatarCollectionUI::RefreshEntries);
	}

	AvatarStorage = TWeakObjectPtr<UAvatarStorage>(AvatarCollection);
}

void UAvatarCollectionUI::DisplayEntries()
{
	UAvatarStorage* AvatarCollection = AvatarStorage.Get();
	if (!IsValid(AvatarCollection))
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarStorage is invalid"));
		return;
	}

	AvatarCollection->QueryItems(GetFilterRoot(), QueryRule,
		[this](const FAvatarSortEntry& SortEntry)
		{
			UAvatarEntry* Entry = GetEntryFromPool<UAvatarEntry>();
			if (IsValid(Entry))
			{
				Entry->AvatarInstance = SortEntry.Item;
				AddEntry(SortEntry.AssetId, Entry);
			}
		}
	);
}

void UAvatarCollectionUI::NativeDestruct()
{
	UAvatarStorage* AvatarCollection = AvatarStorage.Get();
	if (IsValid(AvatarCollection))
	{
		AvatarCollection->OnRefreshed.RemoveAll(this);
	}
	AvatarStorage.Reset();

	Super::NativeDestruct();
}

