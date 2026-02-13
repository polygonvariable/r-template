// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TaskObject.h"

// Engine Headers

// Project Headers



void UTaskObject::StartTask()
{
	Callback.ExecuteIfBound(ETaskState::Started, TEXT(""));

	OnStarted();
}

void UTaskObject::StopTask()
{
	OnStopped();

	Callback.ExecuteIfBound(ETaskState::Stopped, TEXT(""));
	OnFinished.ExecuteIfBound(TaskId);
	Cleanup();
}

void UTaskObject::Succeed()
{
	Callback.ExecuteIfBound(ETaskState::Finished, TEXT(""));
	OnFinished.ExecuteIfBound(TaskId);
	Cleanup();
}

void UTaskObject::Fail(const FString& Reason)
{
	Callback.ExecuteIfBound(ETaskState::Failed, Reason);
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

