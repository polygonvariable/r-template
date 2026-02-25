// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeDashboardUI.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Filter/FilterLeafCriterion.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/TradeCollectionUI.h"
#include "Widget/TradeDetailUI.h"
#include "Widget/TradeEntry.h"



void UTradeDashboardUI::SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UTradeEntry* ShopEntry = Cast<UTradeEntry>(Entry);
	if (!IsValid(Entry))
	{
		return;
	}

	const FPrimaryAssetId& CostAssetId = ShopEntry->CostAssetId;
	const FAssetDetail& CostItem = ShopEntry->CostItem;

	UFilterAssetCriterion* AssetCriterion = SecondaryCollection->GetCriterionByName<UFilterAssetCriterion>(AssetFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		AssetCriterion->Included.Empty();
		AssetCriterion->Included.Add(CostAssetId);
	}

	SecondaryCollection->ClearSubDetails();
	SecondaryCollection->AddSubDetails(CostAssetId, FInstancedStruct::Make(CostItem));
	SecondaryCollection->RefreshEntries();
}

void UTradeDashboardUI::GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const
{
	OutAssetUI.Add(PrimaryDetail);
}

void UTradeDashboardUI::NativePreConstruct()
{
	PrimaryCollection->SetTradeAssetId(TradeAssetId);
}

void UTradeDashboardUI::NativeConstruct()
{
	PrimaryCollection->OnEntrySelected.BindUObject(this, &UTradeDashboardUI::InitializeDetails);
	PrimaryCollection->DisplayEntries();

	Super::NativeConstruct();
}

void UTradeDashboardUI::NativeDestruct()
{
	PrimaryCollection->OnEntrySelected.Unbind();

	Super::NativeDestruct();
}

