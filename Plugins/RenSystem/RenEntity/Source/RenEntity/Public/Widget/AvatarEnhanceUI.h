// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogUI.h"

// Generated Headers
#include "AvatarEnhanceUI.generated.h"

// Forward Declarations
class UButton;

class UAvatarDetailUI;
class UCatalogEntry;
class UCatalogCollectionUI;
class UAvatarAscensionSubsystem;



/**
 *
 */
UCLASS(Abstract)
class UAvatarEnhanceUI : public UCatalogUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UCatalogCollectionUI> ItemCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UAvatarDetailUI> AvatarDetail = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;

	UPROPERTY()
	TObjectPtr<UAvatarAscensionSubsystem> AscensionSubsystem = nullptr;



	UFUNCTION()
	void AddExperiencePoints(const UCatalogEntry* Entry);

	UFUNCTION()
	void AddRankPoints();

	UFUNCTION()
	virtual void HandleItemSelected(const UCatalogEntry* Entry);

	// ~ UCatalogUI
	virtual void SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	// ~ End of UCatalogUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};
