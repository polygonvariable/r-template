// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/InventoryQueryType.h"
#include "Widget/AssetCollectionUI.h"

// Generated Headers
#include "InventoryCollectionUI.generated.h"

// Module Macros

// Forward Declarations
class UInventorySubsystem;



/**
 *
 */
UCLASS(Abstract)
class UInventoryCollectionUI : public UAssetCollectionUI
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionUI
	virtual void DisplayEntries() override;
	// ~ End of UAssetCollectionUI

protected:

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	UPROPERTY(EditAnywhere)
	FInventoryQueryRule QueryRule;

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem = nullptr;


	void OnInventoryRefreshed(const FGuid& InventoryId);

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};



// Module Macros

