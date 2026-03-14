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

	UAvatarStorage* AvatarStorage = AvatarSubsystem->GetAvatarCollection(AssetSourceId);
	if (IsValid(AvatarStorage) && bAutoRefresh)
	{
		AvatarStorage->OnRefreshed.AddUObject(this, &UAvatarCollectionUI::RefreshEntries);
	}

	AvatarCollection = TWeakObjectPtr<UAvatarStorage>(AvatarStorage);
}

void UAvatarCollectionUI::DisplayEntries()
{
	UAvatarStorage* AvatarStorage = AvatarCollection.Get();
	if (!IsValid(AvatarStorage))
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarStorage is invalid"));
		return;
	}

	AvatarStorage->QueryItems(GetFilterRoot(), QueryRule,
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
	UAvatarStorage* AvatarStorage = AvatarCollection.Get();
	if (IsValid(AvatarStorage))
	{
		AvatarStorage->OnRefreshed.RemoveAll(this);
	}
	AvatarCollection.Reset();

	Super::NativeDestruct();
}

