// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/ItemAscensionSubsystem.h"

// Engine Headers

// Project Headers
#include "Delegate/LatentDelegate.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Task/GrantItemExperience.h"
#include "Task/GrantItemRank.h"
#include "Subsystem/TaskSubsystem.h"



void UItemAscensionSubsystem::AddExperiencePoints(FGuid TaskId, const FGuid& InventoryId, const FPrimaryAssetId& TargetAssetId, const FGuid& TargetId, const FPrimaryAssetId& MaterialAssetId, const FGuid& MaterialId, FTaskCallback Callback)
{
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogItemAscension, TEXT("Task subsystem is invalid"));
		return;
	}

	UGrantItemExperience* Task = TaskSubsystem->CreateTask<UGrantItemExperience>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogItemAscension, TEXT("Failed to create task"));
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

void UItemAscensionSubsystem::AddRankPoints(FGuid TaskId, const FGuid& InventoryId, const FPrimaryAssetId& TargetAssetId, const FGuid& TargetId, FTaskCallback Callback)
{
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogItemAscension, TEXT("Task subsystem is invalid"));
		return;
	}

	UGrantItemRank* Task = TaskSubsystem->CreateTask<UGrantItemRank>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogItemAscension, TEXT("Failed to create task"));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->InventoryId = InventoryId;
	Task->TargetAssetId = TargetAssetId;
	Task->TargetId = TargetId;
	Task->StartTask();
}

void UItemAscensionSubsystem::OnPreGameInitialized()
{
	FLatentDelegate::OnPreGameInitialized.RemoveAll(this);

	TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
}

bool UItemAscensionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UItemAscensionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FLatentDelegate::OnPreGameInitialized.AddUObject(this, &UItemAscensionSubsystem::OnPreGameInitialized);

	LOG_WARNING(LogItemAscension, TEXT("ItemAscensionSubsystem initialized"));
}

void UItemAscensionSubsystem::Deinitialize()
{
	FLatentDelegate::OnPreGameInitialized.RemoveAll(this);

	TaskSubsystem = nullptr;

	LOG_WARNING(LogItemAscension, TEXT("ItemAscensionSubsystem deinitialized"));
	Super::Deinitialize();
}


UItemAscensionSubsystem* UItemAscensionSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UItemAscensionSubsystem* UItemAscensionSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UItemAscensionSubsystem>();
}

