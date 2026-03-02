// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeDashboardUI.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Filter/FilterLeafCriterion.h"
#include "Management/Collection/AssetCollectionUnique.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/TradeCollectionUI.h"
#include "Widget/TradeDetailUI.h"
#include "Widget/TradeEntry.h"



void UTradeDashboardUI::SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UTradeEntry* TradeEntry = Cast<UTradeEntry>(Entry);
	if (!IsValid(TradeEntry))
	{
		return;
	}

	const UAssetCollection* CostCollection = TradeEntry->CostCollection.Get();
	if (!IsValid(CostCollection))
	{
		return;
	}

	SecondaryCollection->ClearSubDetails();

	UFilterAssetCriterion* AssetCriterion = SecondaryCollection->GetCriterionByName<UFilterAssetCriterion>(AssetFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		AssetCriterion->Included.Empty();
	
		TMap<FPrimaryAssetId, FAssetDetail> CostList;
		CostCollection->GetAssetList(CostList);

		for (const TPair<FPrimaryAssetId, FAssetDetail>& AssetKv : CostList)
		{
			const FPrimaryAssetId& AssetId = AssetKv.Key;

			AssetCriterion->Included.Add(AssetId);
			SecondaryCollection->AddSubDetails(AssetId, FInstancedStruct::Make(AssetKv.Value));
		}
	}

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

