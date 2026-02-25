// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetEntryUI.generated.h"

// Module Macros
#define RCORE_API RCOREASSETUI_API

// Forward Declarations
class UImage;
class UTextBlock;

struct FInstancedStruct;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetEntryUI : public UAssetUI, public IUserObjectListEntry
{

	GENERATED_BODY()

public:

	// ~ UAssetUI
	RCORE_API virtual void ResetDetails() override;
	// ~ End of UAssetUI

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> EntryIcon = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryName = nullptr;


	UFUNCTION(BlueprintCallable)
	void GetAssetDetail(FInstancedStruct& AssetDetail) const;


	// ~ IUserObjectListEntry
	RCORE_API virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	RCORE_API virtual void NativeOnItemSelectionChanged(bool bSelected) override;
	// ~ End of IUserObjectListEntry

};



// Module Macros
#undef RCORE_API

