// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetCollectionUI.h"

// Generated Headers
#include "AvatarCollectionUI.generated.h"

// Forward Declarations
class UAvatarSubsystem;



/**
 *
 */
UCLASS(Abstract)
class UAvatarCollectionUI : public UAssetCollectionUI
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionUI
	virtual void DisplayEntries() override;
	// ~ End of UAssetCollectionUI

protected:

	UPROPERTY()
	TWeakObjectPtr<UAvatarSubsystem> AvatarSubsystem = nullptr;

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

