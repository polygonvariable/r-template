// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventoryStorageSubsystem.h"

// Engine Headers

// Project Headers
#include "LatentDelegates.h"
#include "LogCategory.h"
#include "LogMacro.h"

#include "InventoryStorage.h"
#include "StorageProviderInterface.h"
#include "SubsystemUtils.h"




void UInventoryStorageSubsystem::WaitForStorage(const FGuid& StorageId, FTaskCallback Callback)
{
	LoadStorage(StorageId, MoveTemp(Callback));
}

void UInventoryStorageSubsystem::MakeQueryString(const TArray<FString>& InQuery, FString& OutString) const
{
	int Count = InQuery.Num();
	if (Count > 0)
	{
		OutString = TEXT("?");
	}

	for (int i = 0; i < Count; i++)
	{
		OutString += InQuery[i];
		if (i < Count - 1)
		{
			OutString += TEXT("&");
		}
	}
}

void UInventoryStorageSubsystem::MakePath(const FGuid& StorageId, FString& OutString)
{
	TArray<FString> Query;
	GetDefaultQuery(Query);

	FString Root = GetRootPath();
	FString QueryString = TEXT("");
	MakeQueryString(Query, QueryString);

	OutString = Root + QueryString;
}

FString UInventoryStorageSubsystem::GetRootPath() const
{
	return TEXT("/api/get/inventory");
}

void UInventoryStorageSubsystem::GetDefaultQuery(TArray<FString>& OutQuery)
{
	OutQuery.Add(FString::Printf(TEXT("storageId=%s"), *GetDefaultStorageId().ToString()));
}

FGuid UInventoryStorageSubsystem::GetDefaultStorageId() const
{
	// TODO:
	// replace this with a setting in which the default storage id
	// can be fetched from that class
	return FGuid(TEXT("24D61E46-5B41-49A4-BDFB-B1E39BF54665"));
}

USaveGame* UInventoryStorageSubsystem::GetStorage(const FGuid& StorageId)
{
	TObjectPtr<USaveGame>* Storage = StorageCollection.Find(StorageId);
	if (!Storage)
	{
		return nullptr;
	}
	return Storage->Get();
}

void UInventoryStorageSubsystem::LoadStorage(const FGuid& StorageId, FTaskCallback Callback)
{
	FString Path;
	MakePath(StorageId, Path);

	if (Path.IsEmpty())
	{
		LOG_ERROR(LogInventory, TEXT("Storage path is empty"));
		Callback.ExecuteIfBound(ETaskState::Failed, TEXT(""));
		return;
	}

	IStorageProviderInterface* StorageProvider = SubsystemUtils::GetSubsystemInterface<IStorageProviderInterface>(GetGameInstance());
	if (!StorageProvider)
	{
		LOG_ERROR(LogInventory, TEXT("Storage provider not found"));
		Callback.ExecuteIfBound(ETaskState::Failed, TEXT(""));
		return;
	}

	UInventoryStorage* Storage = StorageProvider->GetStorage<UInventoryStorage>(*Path);
	if (!IsValid(Storage))
	{
		LOG_ERROR(LogInventory, TEXT("Inventory storage not found"));
		Callback.ExecuteIfBound(ETaskState::Failed, TEXT(""));
		return;
	}

	StorageProviderInterface = TWeakInterfacePtr<IStorageProviderInterface>(StorageProvider);
	StorageCollection.Add(StorageId, Storage);
	PathCollection.Add(StorageId, Path);

	Callback.ExecuteIfBound(ETaskState::Finished, TEXT(""));
}

void UInventoryStorageSubsystem::SaveStorage(FTaskCallback Callback)
{
	IStorageProviderInterface* StorageProvider = StorageProviderInterface.Get();
	if (StorageProvider)
	{
		for (const TPair<FGuid, TObjectPtr<USaveGame>>& Pair : StorageCollection)
		{
			const FString Path = PathCollection.FindRef(Pair.Key);
			if (Path.IsEmpty())
			{
				continue;
			}
			StorageProvider->SaveStorage(Pair.Value, *Path);
		}
	}

	PathCollection.Empty();
	StorageCollection.Empty();
	StorageProviderInterface.Reset();

	Callback.ExecuteIfBound(ETaskState::Finished, TEXT(""));
}


void UInventoryStorageSubsystem::OnGameLoaded()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	LoadStorage(GetDefaultStorageId(), FTaskCallback());
}

bool UInventoryStorageSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventoryStorageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventory, TEXT("InventoryStorageSubsystem initialized"));

	FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UInventoryStorageSubsystem::OnGameLoaded);
}

void UInventoryStorageSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	SaveStorage(FTaskCallback());

	LOG_WARNING(LogInventory, TEXT("InventoryStorageSubsystem deinitialized"));
	Super::Deinitialize();
}

