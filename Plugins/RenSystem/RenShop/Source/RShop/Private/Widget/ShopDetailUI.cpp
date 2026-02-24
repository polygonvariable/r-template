// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"



void UShopDetailUI::SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}

	EntryName->SetText(Asset->DisplayName);
	EntryDescription->SetText(Asset->Description);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
}

void UShopDetailUI::SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{

}

void UShopDetailUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UShopDetailUI::NativeDestruct()
{
	Super::NativeDestruct();
}

