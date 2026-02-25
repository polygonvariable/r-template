// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetDetailUI.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetDetailUI : public UAssetUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
	bool bAutoRefresh = false;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> EntryIcon = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryName = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryDescription = nullptr;

};

