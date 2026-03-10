// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CraftCollectionUI.h"

// Engine Headers

// Project Headers
#include "Asset/TradeAsset.h"
#include "Storage/CraftStorage.h"
#include "Subsystem/CraftSubsystem.h"
#include "Widget/CraftEntry.h"



void UCraftCollectionUI::DisplayEntries()
{
	if (!IsValid(CraftSubsystem))
	{
		return;
	}

	FPrimaryAssetId TradeAssetId = TradeAsset->GetPrimaryAssetId();

	CraftSubsystem->QueryItems(TradeAsset, TradeCollectionId, QuerySource,
		[this, TradeAssetId](const FPrimaryAssetId& ItemAssetId, const FAssetDetail_Trade& ItemDetail, const FCraftData* CraftData) {

			UCraftEntry* Entry = GetEntryFromPool<UCraftEntry>();
			if (IsValid(Entry))
			{
				Entry->CraftData = (CraftData) ? *CraftData : FCraftData();
				Entry->TradeDetail = ItemDetail;
				AddEntry(ItemAssetId, Entry);
			}

		}
	);
}

void UCraftCollectionUI::NativeConstruct()
{
	CraftSubsystem = UCraftSubsystem::Get(GetGameInstance());
	if (IsValid(CraftSubsystem))
	{
		UCraftStorage* CraftStorage = CraftSubsystem->GetCraftStorage();
		if (IsValid(CraftStorage))
		{
			CraftStorage->OnCraftUpdated.AddUObject(this, &UCraftCollectionUI::RefreshEntries);
		}
	}

	Super::NativeConstruct();
}

void UCraftCollectionUI::NativeDestruct()
{
	if (IsValid(CraftSubsystem))
	{
		UCraftStorage* CraftStorage = CraftSubsystem->GetCraftStorage();
		if (IsValid(CraftStorage))
		{
			CraftStorage->OnCraftUpdated.RemoveAll(this);
		}
	}
	CraftSubsystem = nullptr;

	Super::NativeDestruct();
}

