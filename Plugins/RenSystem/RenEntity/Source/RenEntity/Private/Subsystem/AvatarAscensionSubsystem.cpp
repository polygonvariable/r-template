// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/AvatarAscensionSubsystem.h"

// Engine Headers

// Project Headers
#include "LatentDelegates.h"
#include "LogCategory.h"
#include "LogMacro.h"
#include "TaskSubsystem.h"

#include "Task/Task_AddAvatarExperience.h"



void UAvatarAscensionSubsystem::AddExperiencePoints(FGuid TaskId, const FPrimaryAssetId& AvatarId, const FPrimaryAssetId& ItemId, FTaskCallback Callback)
{
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogAvatar, TEXT("Task subsystem is invalid"));
		return;
	}

	UTask_AddAvatarExperience* Task = TaskSubsystem->CreateTask<UTask_AddAvatarExperience>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogAvatar, TEXT("Failed to create task"));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->AvatarId = AvatarId;
	Task->ItemId = ItemId;
	Task->StartTask();
}

void UAvatarAscensionSubsystem::AddRankPoints(FGuid TaskId, const FPrimaryAssetId& AvatarId)
{
	
}

void UAvatarAscensionSubsystem::OnGameLoaded()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		TaskSubsystem = GameInstance->GetSubsystem<UTaskSubsystem>();
	}
}

bool UAvatarAscensionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UAvatarAscensionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UAvatarAscensionSubsystem::OnGameLoaded);

	LOG_WARNING(LogAvatar, TEXT("AvatarAscensionSubsystem initialized"));
}

void UAvatarAscensionSubsystem::Deinitialize()
{
	LOG_WARNING(LogAvatar, TEXT("AvatarAscensionSubsystem deinitialized"));
	Super::Deinitialize();
}

