// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TaskObject.h"

// Engine Headers

// Project Headers



void UTaskObject::StartTask()
{
	Callback.ExecuteIfBound(FTaskResult(ETaskState::Pending));
	OnStarted();
}

void UTaskObject::StopTask()
{
	OnStopped();
	Callback.ExecuteIfBound(FTaskResult(ETaskState::Cancelled));
	OnFinished.ExecuteIfBound(TaskId);
	Cleanup();
}

void UTaskObject::Succeed()
{
	Callback.ExecuteIfBound(FTaskResult(ETaskState::Completed));
	OnFinished.ExecuteIfBound(TaskId);
	Cleanup();
}

void UTaskObject::Fail(const FString& Reason)
{
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

