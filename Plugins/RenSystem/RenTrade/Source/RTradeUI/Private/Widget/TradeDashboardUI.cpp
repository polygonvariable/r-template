// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeDashboardUI.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Filter/FilterLeafCriterion.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Manager/RAssetManager.inl"
#include "Widget/AssetCollectionUI.h"
#include "Widget/TradeCollectionUI.h"
#include "Widget/TradeDetailUI.h"
#include "Widget/TradeEntry.h"



void UTradeDashboardUI::ResetDetail()
{
	PrimaryDetail->ResetDetail();
	SecondaryCollection->ClearEntries(true);
}


const UAssetCollection* UTradeDashboardUI::GetTradeMaterialCollection(const URPrimaryDataAsset* Asset) const
{
	return nullptr;
}

void UTradeDashboardUI::InitializeTradeDetails(const UTradeAsset* Asset)
{
	PrimaryCollection->TradeAsset = Asset;
	PrimaryCollection->TradeCollectionId = TradeCollectionId;
	PrimaryCollection->InitializeCollection();
	PrimaryCollection->DisplayEntries();

	PrimaryDetail->TradeCollectionId = TradeCollectionId;
	PrimaryDetail->TradeAssetId = TradeAssetId;
	PrimaryDetail->InitializeDetail();

	SecondaryCollection->InitializeCollection();
}


void UTradeDashboardUI::InitializeDetail()
{
	if (!IsValid(AssetManager) || !TradeAssetId.IsValid())
	{
		LOG_ERROR(LogAsset, TEXT("Entry, AssetManager or AssetId is invalid"));
		return;
	}

	AssetManager->CancelFetch(_TradeLoadId);

	_TradeLoadId = FGuid::NewGuid();

	TFuture<FLatentLoadedAsset<UTradeAsset>> Future = AssetManager->FetchPrimaryAsset<UTradeAsset>(_TradeLoadId, TradeAssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogAsset, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UTradeDashboardUI> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAsset<UTradeAsset>& Result)
		{
			UTradeDashboardUI* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->InitializeTradeDetails(Result.Get());
			}
		}
	);
}

void UTradeDashboardUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	PrimaryDetail->InitializeDetail(Entry, Asset);
}

void UTradeDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UTradeEntry* TradeEntry = Cast<UTradeEntry>(Entry);
	const UAssetCollection* MaterialCollection = GetTradeMaterialCollection(Asset);
	if (!IsValid(TradeEntry) || !IsValid(MaterialCollection))
	{
		return;
	}

	SecondaryCollection->ClearSubDetails();

	UFilterAssetCriterion* AssetCriterion = SecondaryCollection->GetCriterionByName<UFilterAssetCriterion>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		AssetCriterion->Included.Empty();
	
		TMap<FPrimaryAssetId, FAssetDetail> MaterialAssetList;
		MaterialCollection->GetAssetList(MaterialAssetList);

		for (const TPair<FPrimaryAssetId, FAssetDetail>& AssetKv : MaterialAssetList)
		{
			const FPrimaryAssetId& AssetId = AssetKv.Key;

			AssetCriterion->Included.Add(AssetId);
			SecondaryCollection->AddSubDetails(AssetId, FInstancedStruct::Make(AssetKv.Value));
		}
	}

	SecondaryCollection->RefreshEntries();
}


void UTradeDashboardUI::NativeConstruct()
{
	PrimaryCollection->OnSelectionChanged.BindUObject(this, &UAssetDashboardUI::InitializeDetail);
	PrimaryCollection->OnSelectionCleared.BindUObject(this, &UAssetDashboardUI::ResetDetail);

	Super::NativeConstruct();
}

void UTradeDashboardUI::NativeDestruct()
{
	if (IsValid(AssetManager))
	{
		AssetManager->CancelFetch(_TradeLoadId);
	}

	PrimaryCollection->OnSelectionChanged.Unbind();
	PrimaryCollection->OnSelectionCleared.Unbind();

	Super::NativeDestruct();
}

