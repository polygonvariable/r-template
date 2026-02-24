// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "InstancedStruct.h"

// Project Headers
#include "Asset/ShopAsset.h"
#include "Definition/AssetFilterProperty.h"
#include "Filter/FilterLeafCriterion.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Subsystem/ShopSubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/ShopCollectionUI.h"
#include "Widget/ShopDetailUI.h"
#include "Widget/ShopEntry.h"



void UShopDashboardUI::SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UShopEntry* ShopEntry = Cast<UShopEntry>(Entry);
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

void UShopDashboardUI::GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const
{
	OutAssetUI.Add(PrimaryDetail);
}

void UShopDashboardUI::HandlePurchase()
{
	const UShopEntry* ShopEntry = PrimaryCollection->GetSelectedEntry<UShopEntry>();
	if (!IsValid(ShopEntry))
	{
		LOG_ERROR(LogInventory, TEXT("ShopEntry is invalid"));
		return;
	}

	const FPrimaryAssetId& ItemAssetId = ShopEntry->AssetId;
	const FPrimaryAssetId& CostAssetId = ShopEntry->CostAssetId;

	UGameInstance* GameInstance = GetGameInstance();
	UShopSubsystem* ShopSubsystem = GameInstance->GetSubsystem<UShopSubsystem>();

	FGuid TaskId = FGuid::NewGuid();

	ShopSubsystem->PurchaseItem(TaskId, ShopAssetId, ItemAssetId, CostAssetId, FTaskCallback::CreateWeakLambda(this,
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

void UShopDashboardUI::NativePreConstruct()
{
	PrimaryCollection->SetShopAssetId(ShopAssetId);
}

void UShopDashboardUI::NativeConstruct()
{
	if (IsValid(PurchaseButton)) PurchaseButton->OnClicked.AddDynamic(this, &UShopDashboardUI::HandlePurchase);

	PrimaryCollection->OnEntrySelected.BindUObject(this, &UShopDashboardUI::InitializeDetails);
	PrimaryCollection->DisplayEntries();

	Super::NativeConstruct();
}

void UShopDashboardUI::NativeDestruct()
{
	if (IsValid(PurchaseButton)) PurchaseButton->OnClicked.RemoveAll(this);

	PrimaryCollection->OnEntrySelected.Unbind();

	Super::NativeDestruct();
}

