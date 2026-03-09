// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Definition/Runtime/CraftData.h"

// Engine Headers

// Project Headers



void FCraftData::Reset()
{
	PendingQuantity = 0;
	BatchStartTimestamp = FDateTime::Now();
	BatchProcessingTime = FTimespan::Zero();
}

bool FCraftData::IsValid() const
{
	return PendingQuantity > 0 && BatchProcessingTime.GetTicks() > 0;
}

void FCraftData::Sanitize()
{
	PendingQuantity = FMath::Max(0, PendingQuantity);
}

FTimespan FCraftData::GetRemainingTime() const
{
	FDateTime EndTime = BatchStartTimestamp + (BatchProcessingTime * PendingQuantity);
	FTimespan Result = EndTime - FDateTime::Now();

	if (Result < FTimespan::Zero())
	{
		Result = FTimespan::Zero();
	}

	return Result;
}

int FCraftData::GetCompletedQuantity() const
{
	if (BatchProcessingTime.GetTicks() <= 0)
	{
		return 0;
	}

	FTimespan Elapsed = FDateTime::Now() - BatchStartTimestamp;
	int Completed = Elapsed.GetTicks() / BatchProcessingTime.GetTicks();

	return FMath::Clamp(Completed, 0, PendingQuantity);
}
