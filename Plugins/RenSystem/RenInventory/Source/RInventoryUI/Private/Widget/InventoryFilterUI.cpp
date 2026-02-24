// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryFilterUI.h"

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Definition/FilterContext.h"
#include "Definition/AssetFilterProperty.h"
#include "Definition/InventoryFilterProperty.h"
#include "Definition/InventoryPrimaryAsset.h"
#include "Filter/FilterCriterion.h"
#include "Filter/FilterGroup.h"
#include "Manager/RAssetManager.h"



void UInventoryFilterUI::SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
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
	Context.SetValue(AssetFilterProperty::AssetId, ActiveAssetId);
	Context.SetValue(InventoryFilterProperty::ItemType, ItemType);
	Context.SetValue(InventoryFilterProperty::ItemRarity, ItemRarity);

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

