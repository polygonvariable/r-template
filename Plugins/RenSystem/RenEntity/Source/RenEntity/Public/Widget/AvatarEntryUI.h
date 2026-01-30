// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "RenEntity/Public/Widget/AvatarUI.h"

// Generated Headers
#include "AvatarEntryUI.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;

class UAvatarAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAvatarEntryUI : public UAvatarUI, public IUserObjectListEntry
{

	GENERATED_BODY()

public:

	// ~ UAvatarUI
	virtual void ResetDetails() override;
	// ~ End of UAvatarUI

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UImage> EntryIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryName = nullptr;

	// ~ UAvatarUI
	virtual void SetPrimaryDetails(const UAvatarAsset* Asset) override;
	virtual void SetSecondaryDetails(const UAvatarAsset* Asset, const FAvatarRecord& Record) override;
	// ~ End of UAvatarUI
	
	// ~ IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bSelected) override;
	// ~ End of IUserObjectListEntry

};

