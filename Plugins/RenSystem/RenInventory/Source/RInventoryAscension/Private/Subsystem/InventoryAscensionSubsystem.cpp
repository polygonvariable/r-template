// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/InventoryAscensionSubsystem.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/GrantItemExperience.h"
#include "Task/GrantItemRank.h"



void UInventoryAscensionSubsystem::AddExperiencePoints(FGuid TaskId, FGuid InventoryId, FPrimaryAssetId TargetAssetId, FGuid TargetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogInventoryAscension, TEXT("Task subsystem is invalid"));
		return;
	}

	UGrantItemExperience* Task = TaskSubsystem->CreateTask<UGrantItemExperience>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogInventoryAscension, TEXT("Failed to create task"));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->InventoryId = InventoryId;
	Task->TargetAssetId = TargetAssetId;
	Task->TargetId = TargetId;
	Task->MaterialAssetId = MaterialAssetId;
	Task->MaterialId = MaterialId;
	Task->StartTask();
}

void UInventoryAscensionSubsystem::AddRankPoints(FGuid TaskId, FGuid InventoryId, FPrimaryAssetId TargetAssetId, FGuid TargetId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogInventoryAscension, TEXT("Task subsystem is invalid"));
		return;
	}

	UGrantItemRank* Task = TaskSubsystem->CreateTask<UGrantItemRank>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogInventoryAscension, TEXT("Failed to create task"));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->InventoryId = InventoryId;
	Task->TargetAssetId = TargetAssetId;
	Task->TargetId = TargetId;
	Task->StartTask();
}




bool UInventoryAscensionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventoryAscensionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventoryAscension, TEXT("InventoryAscensionSubsystem initialized"));
}

void UInventoryAscensionSubsystem::Deinitialize()
{
	LOG_WARNING(LogInventoryAscension, TEXT("InventoryAscensionSubsystem deinitialized"));
	Super::Deinitialize();
}



UInventoryAscensionSubsystem* UInventoryAscensionSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UInventoryAscensionSubsystem* UInventoryAscensionSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UInventoryAscensionSubsystem>();
}

