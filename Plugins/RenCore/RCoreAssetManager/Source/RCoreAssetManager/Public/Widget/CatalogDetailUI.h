// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogUI.h"

// Generated Headers
#include "CatalogDetailUI.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UCatalogDetailUI : public UCatalogUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UImage> EntryIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryName = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryDescription = nullptr;

};

