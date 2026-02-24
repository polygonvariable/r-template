// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AvatarViewUI.generated.h"

// Forward Declarations
class UButton;

class UAvatarCollectionUI;
class UAvatarDetailUI;
class UAssetEntry;
class UAssetCollectionUI;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract)
class UAvatarViewUI : public UAssetUI
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
	TSubclassOf<UAssetUI> EnhanceWidgetClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> EnhanceButton = nullptr;


	TWeakObjectPtr<const UAssetEntry> ActiveEntry = nullptr;
	TWeakObjectPtr<const URPrimaryDataAsset> ActiveAsset = nullptr;


	UFUNCTION()
	void HandleEnhanceClicked();

	// ~ UAssetUI
	virtual void SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetUI

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

