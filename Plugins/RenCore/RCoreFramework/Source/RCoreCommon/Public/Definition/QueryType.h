// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers

// Forward Declarations


/**
 *
 */
UENUM(BlueprintType)
enum class EFilterOperator : uint8
{
	Or UMETA(DisplayName = "Or"),
	And UMETA(DisplayName = "And"),
};

/**
 *
 */
UENUM(BlueprintType)
enum class ESortDirection : uint8
{
	Ascending UMETA(DisplayName = "Ascending"),
	Descending UMETA(DisplayName = "Descending"),
};

