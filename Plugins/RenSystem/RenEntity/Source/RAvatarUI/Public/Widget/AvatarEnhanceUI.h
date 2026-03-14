// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AvatarEnhanceUI.generated.h"

// Forward Declarations
class UButton;
class UAvatarDetailUI;
class UAssetEntry;
class UAssetCollectionUI;
class UAvatarAscensionSubsystem;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract)
class UAvatarEnhanceUI : public UAssetUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> ItemCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UAvatarDetailUI> AvatarDetail = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;

	UPROPERTY()
	TObjectPtr<UAvatarAscensionSubsystem> AscensionSubsystem = nullptr;



	UFUNCTION()
	void AddExperiencePoints(const UAssetEntry* Entry);

	UFUNCTION()
	void AddRankPoints();

	UFUNCTION()
	virtual void HandleItemSelected(const UAssetEntry* Entry);

	// ~ UAssetUI
	virtual void SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};
