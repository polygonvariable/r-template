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



const UAssetCollection* UTradeDashboardUI::GetMaterialCollection(const URPrimaryDataAsset* Asset) const
{
	return nullptr;
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
				This->OnTradeAssetLoaded(Result.Get());
			}
		}
	);
}

void UTradeDashboardUI::OnTradeAssetLoaded(const UTradeAsset* Asset)
{
	PrimaryCollection->SetTradeAsset(Asset);
	PrimaryCollection->SetTradeCollectionId(TradeCollectionId);
	PrimaryCollection->InitializeCollection();
	PrimaryCollection->DisplayEntries();

	SecondaryCollection->InitializeCollection();
}



void UTradeDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UTradeEntry* TradeEntry = Cast<UTradeEntry>(Entry);
	if (!IsValid(TradeEntry))
	{
		return;
	}

	const UAssetCollection* MaterialCollection = GetMaterialCollection(Asset);
	if (!IsValid(MaterialCollection))
	{
		return;
	}

	SecondaryCollection->ClearSubDetails();

	UFilterAssetCriterion* AssetCriterion = SecondaryCollection->GetCriterionByName<UFilterAssetCriterion>(AssetFilterProperty::AssetId);
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

void UTradeDashboardUI::GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const
{
	OutAssetUI.Add(PrimaryDetail);
}

void UTradeDashboardUI::NativeConstruct()
{
	PrimaryCollection->OnEntrySelected.BindUObject(this, &UAssetDashboardUI::InitializeDetail);

	Super::NativeConstruct();
}

void UTradeDashboardUI::NativeDestruct()
{
	if (IsValid(AssetManager))
	{
		AssetManager->CancelFetch(_TradeLoadId);
	}

	PrimaryCollection->OnEntrySelected.Unbind();

	Super::NativeDestruct();
}

