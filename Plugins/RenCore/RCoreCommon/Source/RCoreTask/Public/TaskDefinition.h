// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers

// Forward Declarations



enum class ETaskState : uint8
{
	Started,
	Stopped,
	Finished,
	Failed,
};

DECLARE_DELEGATE_OneParam(FOnTaskFinished, FGuid);
DECLARE_DELEGATE_TwoParams(FTaskCallback, ETaskState, const FString&);

