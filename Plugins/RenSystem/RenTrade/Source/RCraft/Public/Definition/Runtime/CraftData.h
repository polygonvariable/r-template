// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "CraftData.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct FCraftData
{

	GENERATED_BODY()

public:

	FCraftData() {}
	FCraftData(int InPendingQuantity, FDateTime InBatchStartTimestamp, FTimespan InBatchProcessingTime)
	{
		PendingQuantity = InPendingQuantity;
		BatchStartTimestamp = InBatchStartTimestamp;
		BatchProcessingTime = InBatchProcessingTime;
	}

	UPROPERTY(SaveGame)
	int PendingQuantity = 0;

	UPROPERTY(SaveGame)
	FDateTime BatchStartTimestamp;

	UPROPERTY(SaveGame)
	FTimespan BatchProcessingTime;


	void Reset();
	bool IsValid() const;
	void Sanitize();

	FTimespan GetRemainingTime() const;
	int GetCompletedQuantity() const;

};

