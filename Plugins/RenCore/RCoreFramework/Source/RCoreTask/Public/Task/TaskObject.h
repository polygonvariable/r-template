// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "TaskObject.generated.h"

// Module Macros
#define RCORE_API RCORETASK_API

// Forward Declarations



/**
 *
 *
 *
 */
UCLASS(Abstract, MinimalAPI)
class UTaskObject : public UObject
{

	GENERATED_BODY()

public:

	FTaskCallback Callback;

	RCORE_API void StartTask();
	RCORE_API void StopTask();

	void Cleanup();

	FOnTaskFinished& GetOnTaskFinished();

	FGuid GetTaskId();
	void SetTaskId(FGuid NewId);

	UPROPERTY()
	TObjectPtr<UTaskObject> NextNode;

protected:

	FOnTaskFinished OnFinished;
	FGuid TaskId;

	RCORE_API virtual void OnStarted() {};
	RCORE_API virtual void OnStopped() {};
	RCORE_API virtual void OnCleanup() {};

	RCORE_API void Success();
	RCORE_API void Fail(const FString& Reason);

};

UCLASS(Abstract, MinimalAPI)
class UNetworkTaskObject : public UTaskObject
{

	GENERATED_BODY()

};



// Module Macros
#undef RCORE_API

