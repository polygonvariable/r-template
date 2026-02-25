// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/MetadataAsset.h"

// Generated Headers
#include "TradeAsset.generated.h"

// Forward Declarations
class UAssetGroup;



/**
 *
 */
UCLASS(MinimalAPI)
class UTradeAsset : public UMetadataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGuid TradeId;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> TradeItems = nullptr;

	// ~ UObject
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UObject

};

