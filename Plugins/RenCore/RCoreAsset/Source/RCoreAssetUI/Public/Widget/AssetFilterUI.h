// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Components/PanelWidget.h"

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetFilterUI.generated.h"

// Module Macros
#define RCORE_API RCOREASSETUI_API

// Forward Declarations
class UNamedSlot;
class UFilterGroup;



/**
 *
 *
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetFilterUI : public UAssetUI
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UFilterGroup> FilterRule = nullptr;

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UNamedSlot> Content = nullptr;

	// ~ UAssetUI
	RCORE_API virtual void SwitchDetail(bool bPrimary) override;
	// ~ End of UAssetUI

};




//UCLASS(BlueprintType, MinimalAPI)
//class UAssetFilter2UI : public UPanelWidget
//{
//
//	GENERATED_BODY()
//
//public:
//
//    UAssetFilter2UI(const FObjectInitializer& ObjectInitializer);
//
//protected:
//
//	virtual TSharedRef<SWidget> RebuildWidget() override;
//	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
//	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;
//	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
//
//private:
//
//	TSharedPtr<SVerticalBox> BasePanel;
//
//};



// Module Macros
#undef RCORE_API

