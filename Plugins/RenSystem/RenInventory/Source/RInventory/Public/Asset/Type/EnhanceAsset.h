// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "Asset/Category/NonEnhanceableAsset.h"

// Generated Headers
#include "EnhanceAsset.generated.h"

// Forward Declarations



/**
 * 
 */
UCLASS(MinimalAPI)
class UEnhanceAsset : public UNonEnhanceableAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int ExchangePoints = 500;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FCurveTableRowHandle ExchangeCurve;

public:

	// ~ IExchangeProviderInterface
	RINVENTORY_API virtual float GetExchangedNumber(FInstancedStruct& Context) const override;
	// ~ End of IExchangeProviderInterface

};

