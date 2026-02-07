// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarCollectionUI.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "RCoreFilter/Public/FilterGroup.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"
#include "RCoreFilter/Public/FilterContext.h"
#include "RCoreFilter/Public/FilterCriterion.h"

#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCorePool/Public/PoolSubsystem.h"

#include "RenEntity/Public/Widget/AvatarEntry.h"
#include "RenEntity/Public/Subsystem/AvatarSubsystem.h"
#include "RenEntity/Public/EntityPrimaryAsset.h"



void UAvatarCollectionUI::DisplayEntries()
{
	UAvatarSubsystem* Avatar = AvatarSubsystem.Get();
	if (!IsValid(Avatar))
	{
		LOG_ERROR(LogAvatar, TEXT("PoolSubsystem, AvatarSubsystem is invalid"));
		return;
	}

	const TMap<FPrimaryAssetId, FAvatarRecord>* AvatarRecords = Avatar->GetAvatarRecords();
	if (!AvatarRecords)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarRecords is invalid"));
		return;
	}

	const UFilterCriterion* Filter = GetFilterRoot();
	
	for (const TPair<FPrimaryAssetId, FAvatarRecord>& Pair : *AvatarRecords)
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
		Entry->AvatarRecord = Pair.Value;

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

