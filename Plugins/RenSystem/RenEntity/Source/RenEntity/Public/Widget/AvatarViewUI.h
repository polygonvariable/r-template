// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogUI.h"

// Generated Headers
#include "AvatarViewUI.generated.h"

// Forward Declarations
class UButton;

class UAvatarCollectionUI;
class UAvatarDetailUI;
class UCatalogEntry;
class UCatalogCollectionUI;



/**
 *
 */
UCLASS(Abstract)
class UAvatarViewUI : public UCatalogUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UAvatarCollectionUI> AvatarCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UAvatarDetailUI> AvatarDetail = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	TSubclassOf<UCatalogUI> EnhanceWidgetClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> EnhanceButton = nullptr;


	TWeakObjectPtr<const UCatalogEntry> ActiveEntry = nullptr;
	TWeakObjectPtr<const UPrimaryDataAsset> ActiveAsset = nullptr;


	UFUNCTION()
	void HandleEnhanceClicked();

	// ~ UCatalogUI
	virtual void SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	// ~ End of UCatalogUI

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

