// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/InventoryAscensionSubsystem.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/Task_GrantItemExperience.h"
#include "Task/Task_GrantItemRank.h"



void UInventoryAscensionSubsystem::AddExperiencePoints(FName SourceId, FPrimaryAssetId TargetAssetId, FGuid TargetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();
	UTask_GrantItemExperience* Task = TaskSubsystem->CreateTask<UTask_GrantItemExperience>(TaskId);
	if (!IsValid(Task))
	{
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->SourceId = SourceId;
	Task->TargetAssetId = TargetAssetId;
	Task->TargetId = TargetId;
	Task->MaterialAssetId = MaterialAssetId;
	Task->MaterialId = MaterialId;
	Task->StartTask();
}

void UInventoryAscensionSubsystem::AddRankPoints(FName SourceId, FPrimaryAssetId TargetAssetId, FGuid TargetId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();
	UTask_GrantItemRank* Task = TaskSubsystem->CreateTask<UTask_GrantItemRank>(TaskId);
	if (!IsValid(Task))
	{
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->SourceId = SourceId;
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

