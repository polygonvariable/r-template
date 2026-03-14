// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetCollectionUI.h"
#include "Definition/AvatarQueryType.h"

// Generated Headers
#include "AvatarCollectionUI.generated.h"

// Forward Declarations
class UAvatarStorage;



/**
 *
 */
UCLASS(Abstract)
class UAvatarCollectionUI : public UAssetCollectionUI
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionUI
	virtual void InitializeCollection() override;
	virtual void DisplayEntries() override;
	// ~ End of UAssetCollectionUI

protected:

	UPROPERTY(EditAnywhere)
	FAvatarQueryRule QueryRule;

	UPROPERTY()
	TWeakObjectPtr<UAvatarStorage> AvatarCollection = nullptr;


	// ~ UUserWidget
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

