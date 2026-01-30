// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEntity/Public/Widget/AvatarUI.h"

// Generated Headers
#include "AvatarDetailUI.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;

class UAvatarSubsystem;
class UAvatarAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAvatarDetailUI : public UAvatarUI
{

	GENERATED_BODY()

public:

	// ~ UInventoryUI
	virtual void ResetDetails() override;
	virtual void RefreshDetails() override;
	// ~ End of UInventoryUI

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UImage> EntryIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryName = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryDescription = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> EntryExperience = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> EntryLevel = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> EntryRank = nullptr;


	// ~ UInventoryUI
	virtual void SetPrimaryDetails(const UAvatarAsset* Asset) override;
	virtual void SetSecondaryDetails(const UAvatarAsset* Asset, const FAvatarRecord& Record) override;
	// ~ End of UInventoryUI

	// ~ End of UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

