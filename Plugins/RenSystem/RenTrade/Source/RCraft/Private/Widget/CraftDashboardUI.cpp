// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CraftDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Subsystem/CraftSubsystem.h"
#include "Widget/TradeCollectionUI.h"
#include "Widget/TradeEntry.h"



void UCraftDashboardUI::HandleCraft()
{
	const UTradeEntry* ShopEntry = PrimaryCollection->GetSelectedEntry<UTradeEntry>();
	if (!IsValid(ShopEntry))
	{
		LOG_ERROR(LogCraft, TEXT("ShopEntry is invalid"));
		return;
	}

	const UAssetCollection* AssetCollection = ShopEntry->CostCollection.Get();
	if (!IsValid(AssetCollection))
	{
		LOG_ERROR(LogCraft, TEXT("AssetCollection is invalid"));
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	UCraftSubsystem* CraftSubsystem = GameInstance->GetSubsystem<UCraftSubsystem>();
	if (!IsValid(CraftSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("CraftSubsystem is invalid"));
		return;
	}

	const FPrimaryAssetId& ItemAssetId = ShopEntry->AssetId;
	const FGameplayTagContainer& CostTags = AssetCollection->GetCollectionTags();

	FGuid TaskId = FGuid::NewGuid();
	CraftSubsystem->CraftItem(TaskId, TradeAssetId, ItemAssetId, CostTags, FTaskCallback::CreateWeakLambda(this,
		[](const FTaskResult& Result)
		{
			if (Result.State == ETaskState::Pending)
			{
				UE_LOG(LogCraft, Log, TEXT("Task Started"));
			}
			else
			{
				UE_LOG(LogCraft, Log, TEXT("Task Finished, Message: %s"), *Result.Message);
			}
		}
	));
}

void UCraftDashboardUI::NativeConstruct()
{
	if (IsValid(CraftButton)) CraftButton->OnClicked.AddDynamic(this, &UCraftDashboardUI::HandleCraft);

	Super::NativeConstruct();
}

void UCraftDashboardUI::NativeDestruct()
{
	if (IsValid(CraftButton)) CraftButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

