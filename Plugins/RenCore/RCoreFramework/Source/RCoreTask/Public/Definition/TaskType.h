// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers

// Forward Declarations



enum class ETaskState : uint8
{
    None,
    Pending,
    Completed,
    Failed,
    Cancelled,
};

struct FTaskResult
{
    FTaskResult(ETaskState InState) : State(InState) {}
    FTaskResult(ETaskState InState, const FString& InMessage) : State(InState), Message(InMessage) {}

    ETaskState State = ETaskState::None;
    FString Message = TEXT_EMPTY;
};

DECLARE_DELEGATE_OneParam(FOnTaskFinished, FGuid);
DECLARE_DELEGATE_OneParam(FTaskCallback, const FTaskResult&);

