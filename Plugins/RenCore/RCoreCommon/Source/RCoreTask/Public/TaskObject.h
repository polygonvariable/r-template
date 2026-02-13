// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "TaskDefinition.h"

// Generated Headers
#include "TaskObject.generated.h"

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

	RCORETASK_API void StartTask();
	RCORETASK_API void StopTask();

	void Cleanup();

	FOnTaskFinished& GetOnTaskFinished();

	FGuid GetTaskId();
	void SetTaskId(FGuid NewId);

	UPROPERTY()
	TObjectPtr<UTaskObject> NextNode;

protected:

	FOnTaskFinished OnFinished;
	FGuid TaskId;

	RCORETASK_API virtual void OnStarted() {};
	RCORETASK_API virtual void OnStopped() {};
	RCORETASK_API virtual void OnCleanup() {};

	RCORETASK_API void Succeed();
	RCORETASK_API void Fail(const FString& Reason);

};

UCLASS(Abstract, MinimalAPI)
class UNetworkTaskObject : public UTaskObject
{

	GENERATED_BODY()

};

