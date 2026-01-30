// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarCollectionUI.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCorePool/Public/PoolSubsystem.h"

#include "RenEntity/Public/Widget/AvatarEntry.h"
#include "RenEntity/Public/Subsystem/AvatarSubsystem.h"




void UAvatarCollectionUI::DisplayEntries()
{
	UPoolSubsystem* Pool = PoolSubsystem.Get();
	UAvatarSubsystem* Avatar = AvatarSubsystem.Get();
	if (!IsValid(Pool) || !IsValid(Avatar) || !IsValid(EntryClass) || !EntityList)
	{
		LOG_ERROR(LogInventory, TEXT("PoolSubsystem, AvatarSubsystem, EntryClass, EntityList is invalid"));
		return;
	}

	const TMap<FPrimaryAssetId, FAvatarRecord>* AvatarRecords = Avatar->GetAvatarRecords();
	if (!AvatarRecords)
	{
		LOG_ERROR(LogInventory, TEXT("AvatarRecords is invalid"));
		return;
	}

	for (const TPair<FPrimaryAssetId, FAvatarRecord>& Pair : *AvatarRecords)
	{
		UAvatarEntry* Entry = Pool->AcquireObject<UAvatarEntry>();
		if (IsValid(Entry))
		{
			Entry->AvatarId = Pair.Key;
			Entry->Record = Pair.Value;

			EntityList->AddItem(Entry);
		}
	}
}

void UAvatarCollectionUI::ClearEntries()
{
	UPoolSubsystem* Pool = PoolSubsystem.Get();
	if (!IsValid(Pool) || !EntityList)
	{
		return;
	}

	const TArray<UObject*>& Items = EntityList->GetListItems();
	for (UObject* Item : Items)
	{
		UAvatarEntry* Entry = Cast<UAvatarEntry>(Item);
		if (!IsValid(Entry))
		{
			continue;
		}
		Entry->ResetData();
		Pool->ReturnToPool<UAvatarEntry>(Entry);
	}

	EntityList->ClearListItems();
	EntityList->RegenerateAllEntries();
}

void UAvatarCollectionUI::RefreshEntries()
{
	
}

UAvatarEntry* UAvatarCollectionUI::GetSelectedEntry()
{
	if (!EntityList)
	{
		return nullptr;
	}
	return EntityList->GetSelectedItem<UAvatarEntry>();
}

void UAvatarCollectionUI::HandleSelectedEntry(UObject* Object)
{
	UAvatarEntry* Entry = Cast<UAvatarEntry>(Object);
	if (IsValid(Entry))
	{
		OnEntrySelected.Broadcast(Entry);
	}
}

void UAvatarCollectionUI::NativeConstruct()
{
	if (EntityList)
	{
		EntityList->OnItemSelectionChanged().RemoveAll(this);
		EntityList->OnItemSelectionChanged().AddUObject(this, &UAvatarCollectionUI::HandleSelectedEntry);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UPoolSubsystem* Pool = GameInstance->GetSubsystem<UPoolSubsystem>();
		UAvatarSubsystem* Avatar = GameInstance->GetSubsystem<UAvatarSubsystem>();

		if (!IsValid(Avatar) || !IsValid(Pool))
		{
			LOG_ERROR(LogAvatar, TEXT("AvatarSubsystem, Pool is invalid"));
			return;
		}

		PoolSubsystem = TWeakObjectPtr<UPoolSubsystem>(Pool);
		AvatarSubsystem = TWeakObjectPtr<UAvatarSubsystem>(Avatar);
	}

	Super::NativeConstruct();
}

void UAvatarCollectionUI::NativeDestruct()
{
	if (EntityList)
	{
		EntityList->OnItemSelectionChanged().RemoveAll(this);
		EntityList->OnItemSelectionChanged().AddUObject(this, &UAvatarCollectionUI::HandleSelectedEntry);
	}

	PoolSubsystem.Reset();
	AvatarSubsystem.Reset();

	Super::NativeDestruct();
}

