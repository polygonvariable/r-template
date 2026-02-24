// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDetailUI.h"

// Generated Headers
#include "AvatarDetailUI.generated.h"

// Forward Declarations
class UTextBlock;
class URPrimaryDataAsset;

struct FAvatarData;


/**
 *
 */
UCLASS(Abstract)
class UAvatarDetailUI : public UAssetDetailUI
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

	// ~ UAssetDetailUI
	virtual void RefreshDetails() override;
	virtual bool IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const override;
	virtual void SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetDetailUI

	virtual void SetSecondaryDetails(const FAvatarData& Entry, const URPrimaryDataAsset* Asset);

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

