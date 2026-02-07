// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogCollectionUI.h"

// Generated Headers
#include "AvatarCollectionUI.generated.h"

// Forward Declarations
class UAvatarSubsystem;



/**
 *
 */
UCLASS(Abstract)
class UAvatarCollectionUI : public UCatalogCollectionUI
{

	GENERATED_BODY()

public:

	// ~ UCatalogCollectionUI
	virtual void DisplayEntries() override;
	// ~ End of UCatalogCollectionUI

protected:

	UPROPERTY()
	TWeakObjectPtr<UAvatarSubsystem> AvatarSubsystem = nullptr;

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

