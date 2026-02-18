// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryFilterUI.h"

// Engine Headers

// Project Headers
#include "Definition/InventoryPrimaryAsset.h"
#include "Definition/InventoryFilterProperty.h"
#include "FilterContext.h"
#include "FilterCriterion.h"
#include "FilterGroup.h"
#include "RAssetManager.h"



bool UInventoryFilterUI::IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const
{
	return InventoryPrimaryAsset::IsValid(AssetId);
}

void UInventoryFilterUI::SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	if (!IsValid(AssetManager) || !IsValid(FilterRule))
	{
		SwitchDetails(false);
		return;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(ActiveAssetId, AssetData))
	{
		SwitchDetails(false);
		return;
	}

	FName ItemType = TEXT_EMPTY;
	InventoryPrimaryAsset::GetType(AssetData, ItemType);

	FName ItemRarity = TEXT_EMPTY;
	InventoryPrimaryAsset::GetRarity(AssetData, ItemRarity);

	FFilterContext Context;
	Context.SetValue(InventoryFilterProperty::AssetId, ActiveAssetId);
	Context.SetValue(InventoryFilterProperty::AssetType, ItemType);
	Context.SetValue(InventoryFilterProperty::AssetRarity, ItemRarity);

	UFilterCriterion* FilterRoot = FilterRule->CriterionRoot;
	if (IsValid(FilterRoot))
	{
		SwitchDetails(FilterRoot->Evaluate(Context));
	}
	else
	{
		SwitchDetails(false);
	}
}

