// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/TaskObject.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


void UTaskObject::StartTask()
{
	LOG_WARNING(LogTask, TEXT("Task started"));

	Callback.ExecuteIfBound(FTaskResult(ETaskState::Pending));
	OnStarted();
}

void UTaskObject::StopTask()
{
	LOG_WARNING(LogTask, TEXT("Task stopped"));

	OnStopped();
	Callback.ExecuteIfBound(FTaskResult(ETaskState::Cancelled));
	OnFinished.ExecuteIfBound(TaskId);
	Cleanup();
}

void UTaskObject::Success()
{
	LOG_INFO(LogTask, TEXT("Task completed"));

	Callback.ExecuteIfBound(FTaskResult(ETaskState::Completed));
	OnFinished.ExecuteIfBound(TaskId);
	Cleanup();
}

void UTaskObject::Fail(const FString& Reason)
{
	LOG_ERROR(LogTask, TEXT("%s"), *Reason);

	Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed, Reason));
	OnFinished.ExecuteIfBound(TaskId);
	Cleanup();
}

void UTaskObject::Cleanup()
{
	OnCleanup();

	Callback.Unbind();
	OnFinished.Unbind();
	TaskId.Invalidate();
}

FOnTaskFinished& UTaskObject::GetOnTaskFinished()
{
	return OnFinished;
}

FGuid UTaskObject::GetTaskId()
{
	return TaskId;
}

void UTaskObject::SetTaskId(FGuid NewId)
{
	if (!TaskId.IsValid())
	{
		TaskId = NewId;
	}
}

