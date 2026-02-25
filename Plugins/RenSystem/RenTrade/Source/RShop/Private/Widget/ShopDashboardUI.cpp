// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/ShopSubsystem.h"
#include "Widget/TradeCollectionUI.h"
#include "Widget/TradeEntry.h"



void UShopDashboardUI::HandlePurchase()
{
	const UTradeEntry* ShopEntry = PrimaryCollection->GetSelectedEntry<UTradeEntry>();
	if (!IsValid(ShopEntry))
	{
		LOG_ERROR(LogInventory, TEXT("ShopEntry is invalid"));
		return;
	}

	const FPrimaryAssetId& ItemAssetId = ShopEntry->AssetId;
	const FPrimaryAssetId& CostAssetId = ShopEntry->CostAssetId;

	UGameInstance* GameInstance = GetGameInstance();
	UShopSubsystem* ShopSubsystem = GameInstance->GetSubsystem<UShopSubsystem>();
	if (!IsValid(ShopSubsystem))
	{
		LOG_ERROR(LogInventory, TEXT("ShopSubsystem is invalid"));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();

	ShopSubsystem->PurchaseItem(TaskId, TradeAssetId, ItemAssetId, CostAssetId, FTaskCallback::CreateWeakLambda(this,
		[](const FTaskResult& Result)
		{
			if (Result.State == ETaskState::Pending)
			{
				UE_LOG(LogAvatar, Log, TEXT("Task Started"));
			}
			else
			{
				UE_LOG(LogAvatar, Log, TEXT("Task Finished, Message: %s"), *Result.Message);
			}
		}
	));
}

void UShopDashboardUI::NativeConstruct()
{
	if (IsValid(PurchaseButton)) PurchaseButton->OnClicked.AddDynamic(this, &UShopDashboardUI::HandlePurchase);

	Super::NativeConstruct();
}

void UShopDashboardUI::NativeDestruct()
{
	if (IsValid(PurchaseButton)) PurchaseButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

