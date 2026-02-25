// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"



void UTradeDetailUI::SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}

	EntryName->SetText(Asset->DisplayName);
	EntryDescription->SetText(Asset->Description);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
}

void UTradeDetailUI::SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{

}

void UTradeDetailUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTradeDetailUI::NativeDestruct()
{
	Super::NativeDestruct();
}

