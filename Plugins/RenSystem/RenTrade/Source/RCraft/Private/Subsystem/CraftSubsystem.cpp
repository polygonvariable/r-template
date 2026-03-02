// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/CraftSubsystem.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/CraftItem.h"



void UCraftSubsystem::CraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FPrimaryAssetId& TargetAssetId, const FGameplayTagContainer& MaterialTags, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("Task subsystem is invalid"));
		return;
	}

	UCraftItem* Task = TaskSubsystem->CreateTask<UCraftItem>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogCraft, TEXT("Failed to create task"));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->CraftAssetId = CraftAssetId;
	Task->TargetAssetId = TargetAssetId;
	Task->MaterialTags = MaterialTags;
	Task->StartTask();
}

bool UCraftSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UCraftSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("CraftSubsystem initialized"));
}

void UCraftSubsystem::Deinitialize()
{
	LOG_WARNING(LogTemp, TEXT("CraftSubsystem deinitialized"));
	Super::Deinitialize();
}



UCraftSubsystem* UCraftSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UCraftSubsystem* UCraftSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UCraftSubsystem>();
}

