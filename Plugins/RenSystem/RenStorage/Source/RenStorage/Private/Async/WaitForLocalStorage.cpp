// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Async/WaitForLocalStorage.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "SaveGame/Storage.h"
#include "Subsystem/StorageSubsystem.h"



UWaitForLocalStorage* UWaitForLocalStorage::WaitForLocalStorage(const UObject* InWorldContext, TSubclassOf<UStorage> InStorageClass, FName InStorageId)
{
	UWaitForLocalStorage* NewTask = NewObject<UWaitForLocalStorage>();
	NewTask->WorldContext = InWorldContext;
    NewTask->StorageClass = InStorageClass;
	NewTask->StorageId = InStorageId;
	return NewTask;
}

void UWaitForLocalStorage::EndTask()
{
	WorldContext.Reset();

	SetReadyToDestroy();
	MarkAsGarbage();
}

UWorld* UWaitForLocalStorage::GetWorld() const
{
	const UObject* Context = WorldContext.Get();
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return Context->GetWorld();
}

void UWaitForLocalStorage::OnStorageLoaded(const FTaskResult& Result)
{
	if (Result.State == ETaskState::Completed)
	{
		OnCompleted.Broadcast(true);
	}
	else if (Result.State == ETaskState::Failed || Result.State == ETaskState::Cancelled)
	{
		OnCompleted.Broadcast(false);
	}

	EndTask();
}

void UWaitForLocalStorage::Activate()
{
	UWorld* World = GetWorld();
	if (!IsValid(World) || !IsValid(StorageClass) || !StorageId.IsValid())
	{
		LOG_ERROR(LogStorage, TEXT("World, StorageClass or StorageId is invalid"));
		OnCompleted.Broadcast(false);
		return;
	}

	UStorageSubsystem* StorageSubsystem = UStorageSubsystem::Get(World);
	if (!StorageSubsystem)
	{
		LOG_ERROR(LogStorage, TEXT("Storage subsystem not found"));
		OnCompleted.Broadcast(false);
		return;
	}

	FStorageHandle Handle;
	Handle.StorageClass = StorageClass;
	Handle.StorageId = StorageId;
	Handle.Callback = FTaskCallback::CreateUObject(this, &UWaitForLocalStorage::OnStorageLoaded);

	StorageSubsystem->LoadStorage(MoveTemp(Handle));
}

