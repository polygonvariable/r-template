// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarCollectionUI.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "FilterGroup.h"
#include "FilterLeafCriterion.h"
#include "FilterContext.h"
#include "FilterCriterion.h"

#include "Log/LogCategory.h"
#include "Log/LogMacro.h"

#include "Widget/AvatarEntry.h"
#include "Subsystem/AvatarSubsystem.h"
#include "EntityPrimaryAsset.h"



void UAvatarCollectionUI::DisplayEntries()
{
	UAvatarSubsystem* Avatar = AvatarSubsystem.Get();
	if (!IsValid(Avatar))
	{
		LOG_ERROR(LogAvatar, TEXT("PoolSubsystem, AvatarSubsystem is invalid"));
		return;
	}

	const TMap<FPrimaryAssetId, FAvatarData>* Collection = Avatar->GetAvatarCollection();
	if (!Collection)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarCollection is invalid"));
		return;
	}

	const UFilterCriterion* Filter = GetFilterRoot();
	
	for (const TPair<FPrimaryAssetId, FAvatarData>& Pair : *Collection)
	{
		if (IsValid(Filter))
		{
			FFilterContext Context;
			Context.SetValue("AssetId", Pair.Key);

			if (!Filter->Evaluate(Context))
			{
				continue;
			}
		}

		UAvatarEntry* Entry = GetEntryFromPool<UAvatarEntry>();
		if (!IsValid(Entry))
		{
			continue;
		}
		Entry->AssetId = Pair.Key;
		Entry->AvatarData = Pair.Value;

		AddEntry(Entry);
	}
}

void UAvatarCollectionUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UAvatarSubsystem* Avatar = GameInstance->GetSubsystem<UAvatarSubsystem>();
		AvatarSubsystem = TWeakObjectPtr<UAvatarSubsystem>(Avatar);
	}

	Super::NativeConstruct();
}

void UAvatarCollectionUI::NativeDestruct()
{
	AvatarSubsystem.Reset();
	Super::NativeDestruct();
}

