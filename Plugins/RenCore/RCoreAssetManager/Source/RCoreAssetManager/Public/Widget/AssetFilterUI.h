// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetFilterUI.generated.h"

// Module Macros
#define RCORE_API RCOREASSETMANAGER_API

// Forward Declarations
class UNamedSlot;
class UFilterGroup;



/**
 *
 *
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetFilterUI : public UAssetUI
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UFilterGroup> FilterRule = nullptr;

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UNamedSlot> Content = nullptr;

	// ~ UAssetUI
	RCORE_API virtual void SwitchDetails(bool bPrimary) override;
	// ~ End of UAssetUI

};



// Module Macros
#undef RCORE_API

