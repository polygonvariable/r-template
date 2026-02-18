// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TaskSubsystem.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "TaskObject.h"





UTaskObject* UTaskSubsystem::AcquireFromList(TSubclassOf<UTaskObject> TaskClass)
{
	TObjectPtr<UTaskObject>* StartingNode = TaskList.Find(TaskClass);
	if (StartingNode)
	{
		UTaskObject* Task = StartingNode->Get();
		if (IsValid(Task))
		{
			TaskList.Add(TaskClass, Task->NextNode);
			Task->NextNode = nullptr;
			return Task;
		}
	}
	return NewObject<UTaskObject>(this, TaskClass);
}

void UTaskSubsystem::ReturnToList(UTaskObject* Task)
{
	if (!IsValid(Task))
	{
		return;
	}

	UClass* TaskClass = Task->GetClass();
	if (TaskList.Contains(TaskClass))
	{
		Task->NextNode = TaskList.FindChecked(TaskClass);
	}
	TaskList.Add(TaskClass, Task);
}


UTaskObject* UTaskSubsystem::CreateTask(FGuid TaskId, TSubclassOf<UTaskObject> TaskClass)
{
	if (ActiveTasks.Contains(TaskId))
	{
		LOG_ERROR(LogTask, TEXT("Latent task already exists"));
		return nullptr;
	}

	UTaskObject* Task = AcquireFromList(TaskClass);
	if (IsValid(Task))
	{
		ActiveTasks.Add(TaskId, Task);

		Task->SetTaskId(TaskId);
		Task->GetOnTaskFinished().BindUObject(this, &UTaskSubsystem::HandleOnTaskFinished);
	}

	return Task;
}

void UTaskSubsystem::StartTask(FGuid TaskId)
{
	UTaskObject* Task = FindTask(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogTask, TEXT("Latent task is invalid"));
		return;
	}

	Task->StartTask();
}

void UTaskSubsystem::StopTask(FGuid TaskId)
{
	UTaskObject* Task = FindTask(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogTask, TEXT("Latent task is invalid"));
		return;
	}

	Task->StopTask();
}

UTaskObject* UTaskSubsystem::FindTask(FGuid TaskId)
{
	TObjectPtr<UTaskObject>* TaskItem = ActiveTasks.Find(TaskId);
	if (!TaskItem)
	{
		return nullptr;
	}

	return TaskItem->Get();
}

void UTaskSubsystem::HandleOnTaskFinished(FGuid TaskId)
{
	UTaskObject* Task = FindTask(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogTask, TEXT("Latent task is invalid"));
		return;
	}

	ActiveTasks.Remove(TaskId);

	ReturnToList(Task);
}

bool UTaskSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UTaskSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LOG_WARNING(LogTask, TEXT("TaskSubsystem initialized"));
}

void UTaskSubsystem::Deinitialize()
{
	LOG_WARNING(LogTask, TEXT("TaskSubsystem deinitialized"));

	Super::Deinitialize();
}


UTaskSubsystem* UTaskSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UTaskSubsystem* UTaskSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UTaskSubsystem>();
}


