// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/PartySubsystem.h"

// Engine Headers

// Project Headers
#include "Component/PartyManagerComponent.h"
#include "Delegate/GameLifecycleDelegates.h"
#include "Interface/IStorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/PartySettings.h"
#include "Storage/PartyStorage.h"
#include "Util/SubsystemUtil.h"



void UPartySubsystem::RegisterManager(UPartyManagerComponent* Manager)
{
	ManagerComponent = TWeakObjectPtr<UPartyManagerComponent>(Manager);
}

void UPartySubsystem::UnregisterManager()
{
	ManagerComponent.Reset();
}

UPartyStorage* UPartySubsystem::GetPartyCollection()
{
	IStorageProvider* StorageInterface = StorageProvider.Get();
	if (!StorageInterface)
	{
		return nullptr;
	}

	FName StorageId = UPartySettings::Get()->StorageId;
	UStorage* Storage = StorageInterface->GetStorage(StorageId);

	return Cast<UPartyStorage>(Storage);
}

void UPartySubsystem::RequestSpawnParty()
{
	UPartyManagerComponent* Manager = ManagerComponent.Get();
	if (IsValid(Manager))
	{
		Manager->SpawnParty();
	}
}

void UPartySubsystem::OnPreGameInitialized()
{
	IStorageProvider* StorageInterface = SubsystemUtil::GetSubsystemInterface<IStorageProvider>(GetGameInstance());
	if (StorageInterface)
	{
		StorageInterface->LoadStorageFromSettings(UPartySettings::Get());
		StorageProvider = TWeakInterfacePtr<IStorageProvider>(StorageInterface);
	}
}

bool UPartySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UPartySettings::Get()->SubsystemClass;
}

void UPartySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogCharacterParty, TEXT("PartySubsystem initialized"));

	FGameLifecycleDelegates::OnPreGameInitialized.AddUObject(this, &UPartySubsystem::OnPreGameInitialized);
}

void UPartySubsystem::Deinitialize()
{
	FGameLifecycleDelegates::OnPreGameInitialized.RemoveAll(this);
	StorageProvider.Reset();

	LOG_WARNING(LogCharacterParty, TEXT("PartySubsystem deinitialized"));
	Super::Deinitialize();
}



UPartySubsystem* UPartySubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UPartySubsystem* UPartySubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UPartySubsystem>();
}

