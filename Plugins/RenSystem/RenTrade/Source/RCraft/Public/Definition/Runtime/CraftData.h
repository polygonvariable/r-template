// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "CraftData.generated.h"

// Module Macros
#define RSYSTEM_API RCRAFT_API



/**
 *
 */
USTRUCT(BlueprintType)
struct FCraftData
{

	GENERATED_BODY()

public:

	FCraftData() {}
	FCraftData(int InPendingQuantity, FDateTime InBatchStartTimestamp, FTimespan InBatchProcessingTime);


	UPROPERTY(SaveGame)
	int PendingQuantity = 0;

	UPROPERTY(SaveGame)
	FDateTime BatchStartTimestamp;

	UPROPERTY(SaveGame)
	FTimespan BatchProcessingTime;


	RSYSTEM_API bool IsValid() const;
	RSYSTEM_API void Reset();
	RSYSTEM_API void Sanitize();

	RSYSTEM_API int GetCompletedQuantity() const;
	RSYSTEM_API FTimespan GetRemainingTime() const;

};



// Module Macros
#undef RSYSTEM_API

