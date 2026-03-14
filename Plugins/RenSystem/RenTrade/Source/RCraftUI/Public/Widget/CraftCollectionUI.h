// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/CraftQuery.h"
#include "Widget/TradeCollectionUI.h"

// Generated Headers
#include "CraftCollectionUI.generated.h"

// Forward Declarations
class UCraftSubsystem;
class UCraftStorage;



/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UCraftCollectionUI : public UTradeCollectionUI
{

	GENERATED_BODY()

protected:

	UPROPERTY()
	TObjectPtr<UCraftSubsystem> CraftSubsystem;

	UPROPERTY()
	TWeakObjectPtr<UCraftStorage> CraftStorage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECraftQuerySource QuerySource;


	// ~ UTradeCollectionUI
	virtual void DisplayEntries() override;
	// ~ End of UTradeCollectionUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

