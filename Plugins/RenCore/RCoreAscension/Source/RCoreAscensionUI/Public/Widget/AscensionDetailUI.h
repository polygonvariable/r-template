// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "AscensionDetailUI.generated.h"

// Module Macros
#define RCORE_API RCOREASCENSIONUI_API

// Forward Declarations
class UTextBlock;

struct FAscensionData;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAscensionDetailUI : public UUserWidget
{

	GENERATED_BODY()

public:
	
    RCORE_API void InitializeDetails(const FAscensionData& Data);

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> ExperienceText = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> RankText = nullptr;
	
};



// Module Macros
#undef RCORE_API

