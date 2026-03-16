// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/AvatarAscensionSubsystem.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/Task_GrantAvatarExperience.h"
#include "Task/Task_GrantAvatarRank.h"



void UAvatarAscensionSubsystem::AddExperiencePoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();
	UTask_GrantAvatarExperience* Task = TaskSubsystem->CreateTask<UTask_GrantAvatarExperience>(TaskId);
	if (!IsValid(Task))
	{
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->TargetSourceId = TargetSourceId;
	Task->TargetAssetId = TargetAssetId;
	Task->MaterialAssetId = MaterialAssetId;
	Task->MaterialId = MaterialId;
	Task->StartTask();
}

void UAvatarAscensionSubsystem::AddRankPoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();
	UTask_GrantAvatarRank* Task = TaskSubsystem->CreateTask<UTask_GrantAvatarRank>(TaskId);
	if (!IsValid(Task))
	{
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->TargetSourceId = TargetSourceId;
	Task->TargetAssetId = TargetAssetId;
	Task->StartTask();
}




bool UAvatarAscensionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UAvatarAscensionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventoryAscension, TEXT("AvatarAscensionSubsystem initialized"));
}

void UAvatarAscensionSubsystem::Deinitialize()
{
	LOG_WARNING(LogInventoryAscension, TEXT("AvatarAscensionSubsystem deinitialized"));
	Super::Deinitialize();
}



UAvatarAscensionSubsystem* UAvatarAscensionSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UAvatarAscensionSubsystem* UAvatarAscensionSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UAvatarAscensionSubsystem>();
}

