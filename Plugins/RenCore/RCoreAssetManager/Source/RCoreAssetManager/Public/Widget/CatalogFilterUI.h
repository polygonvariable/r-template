// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogUI.h"

// Generated Headers
#include "CatalogFilterUI.generated.h"

// Forward Declarations
class UNamedSlot;
class UFilterGroup;



/**
 *
 *
 *
 */
UCLASS(Abstract, MinimalAPI)
class UCatalogFilterUI : public UCatalogUI
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UFilterGroup> FilterRule = nullptr;

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UNamedSlot> Content = nullptr;

	// ~ UCatalogUI
	RCOREASSETMANAGER_API virtual void SwitchDetails(bool bPrimary) override;
	// ~ End of UCatalogUI

};


