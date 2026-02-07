// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers

// Generated Headers

// Forward Declarations


UENUM(BlueprintType)
enum class ESelectionCondition : uint8
{
	Or UMETA(DisplayName = "Or"),
	And UMETA(DisplayName = "And"),
};

