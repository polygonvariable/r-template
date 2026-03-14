// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CraftDetailUI.h"

// Engine Headers
#include "Components/TextBlock.h"

// Project Headers
#include "Definition/Runtime/TradeKey.h"
#include "Storage/CraftStorage.h"
#include "Subsystem/CraftSubsystem.h"
#include "Widget/CraftEntry.h"



void UCraftDetailUI::RefreshDetail()
{
	UCraftStorage* Craft = CraftStorage.Get();
	if (!IsValid(Craft))
	{
		SwitchDetail(false);
		return;
	}

	FTradeKey TradeKey(TradeAssetId, TradeCollectionId, GetActiveAssetId());
	const FCraftData* NewCraftData = Craft->GetItem(TradeKey);
	if (!NewCraftData)
	{
		SwitchDetail(false);
		return;
	}

	CraftData = *NewCraftData;

	RefreshCraftDetail();
}

void UCraftDetailUI::RefreshCraftDetail()
{
	CraftQuantity->SetText(FText::AsNumber(CraftData.GetCompletedQuantity()));

	FTimespan RemainingTime = CraftData.GetRemainingTime();
	if (RemainingTime.IsZero())
	{
		CraftTime->SetText(FText::FromString(TEXT("Completed")));
		return;
	}

	CraftTime->SetText(FText::AsNumber(RemainingTime.GetSeconds()));
}

void UCraftDetailUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UCraftEntry* CraftEntry = Cast<UCraftEntry>(Entry);
	if (!IsValid(CraftEntry))
	{
		SwitchDetail(false);
		return;
	}

	CraftData = CraftEntry->CraftData;

	RefreshCraftDetail();
}

void UCraftDetailUI::NativeConstruct()
{
	UCraftSubsystem* CraftSubsystem = UCraftSubsystem::Get(GetGameInstance());
	if (IsValid(CraftSubsystem))
	{
		UCraftStorage* Craft = CraftSubsystem->GetCraft(AssetSourceId);
		if (IsValid(Craft))
		{
			Craft->OnCraftUpdated.AddUObject(this, &UCraftDetailUI::RefreshDetail);
		}
		CraftStorage = TWeakObjectPtr<UCraftStorage>(Craft);
	}

	Super::NativeConstruct();
}

void UCraftDetailUI::NativeDestruct()
{
	UCraftStorage* Craft = CraftStorage.Get();
	if (IsValid(Craft))
	{
		Craft->OnCraftUpdated.RemoveAll(this);
	}
	CraftStorage.Reset();

	Super::NativeDestruct();
}

