// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "InventoryDefinition.h"
#include "Widget/CatalogCollectionUI.h"

// Generated Headers
#include "InventoryCollectionUI.generated.h"

// Forward Declarations
class UInventorySubsystem;



/**
 *
 */
UCLASS(Abstract)
class UInventoryCollectionUI : public UCatalogCollectionUI
{

	GENERATED_BODY()

public:

	void SetContainerId(FGuid Id);

	// ~ UCatalogCollectionUI
	virtual void DisplayEntries() override;
	// ~ End of UCatalogCollectionUI

protected:

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	UPROPERTY(EditAnywhere)
	FGuid ContainerId;

	UPROPERTY(EditAnywhere)
	FInventoryQueryRule QueryRule = FInventoryQueryRule();

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;


	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

