// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "Widget/CatalogUI.h"

// Generated Headers
#include "CatalogEntryUI.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;

class UCatalogEntry;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UCatalogEntryUI : public UCatalogUI, public IUserObjectListEntry
{

	GENERATED_BODY()

public:

	// ~ UCatalogUI
	RCOREASSETMANAGER_API virtual void ResetDetails() override;
	// ~ End of UCatalogUI

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UImage> EntryIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryName = nullptr;


	// ~ IUserObjectListEntry
	RCOREASSETMANAGER_API virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	RCOREASSETMANAGER_API virtual void NativeOnItemSelectionChanged(bool bSelected) override;
	// ~ End of IUserObjectListEntry

};

