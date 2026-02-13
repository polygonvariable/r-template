// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogDetailUI.h"

// Generated Headers
#include "AvatarDetailUI.generated.h"

// Forward Declarations
class UTextBlock;

struct FAvatarData;


/**
 *
 */
UCLASS(Abstract)
class UAvatarDetailUI : public UCatalogDetailUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryExperience = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryLevel = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryRank = nullptr;


	UFUNCTION()
	virtual void HandleAvatarDataChanged(const FPrimaryAssetId& AssetId);

	// ~ UCatalogDetailUI
	virtual void RefreshDetails() override;
	virtual bool IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const override;
	virtual void SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	// ~ End of UCatalogDetailUI

	virtual void SetSecondaryDetails(const FAvatarData& Entry, const UPrimaryDataAsset* Asset);

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

