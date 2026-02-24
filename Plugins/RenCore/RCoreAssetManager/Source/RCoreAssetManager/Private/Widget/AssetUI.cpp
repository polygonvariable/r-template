// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetUI.h"

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Widget/AssetEntry.h"




void UAssetUI::SetCatalogId(const FGuid& Id)
{
	CatalogId = Id;
}

void UAssetUI::InitializeDetails(const UAssetEntry* Entry)
{
	LatentId = FGuid::NewGuid();

	if (!IsValid(Entry))
	{
		LOG_ERROR(LogAsset, TEXT("Entry is invalid"));
		return;
	}

	ActiveAssetId = Entry->AssetId;
	if (!LatentId.IsValid() || !IsValid(AssetManager))
	{
		LOG_ERROR(LogAsset, TEXT("AssetId, LatentId, AssetManager is invalid"));
		return;
	}

	TFuture<FLatentResultAsset<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAsset<URPrimaryDataAsset>(LatentId, ActiveAssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogAsset, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<const UAssetEntry> WeakEntry(Entry);
	TWeakObjectPtr<UAssetUI> WeakThis(this);

	Future.Next([WeakThis, WeakEntry](const FLatentResultAsset<URPrimaryDataAsset>& Result)
		{
			UAssetUI* This = WeakThis.Get();
			const UAssetEntry* Entry = WeakEntry.Get();
			if (IsValid(This) && Result.IsValid())
			{
				const URPrimaryDataAsset* Asset = Result.GetAsset();
				This->SetPrimaryDetails(Entry, Asset);
				This->SetSecondaryDetails(Entry, Asset);
			}
		}
	);
}

void UAssetUI::InitializeDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	if (IsValid(Asset))
	{
		ActiveAssetId = Asset->GetPrimaryAssetId();
	}

	SetPrimaryDetails(Entry, Asset);
	SetSecondaryDetails(Entry, Asset);
}

bool UAssetUI::IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const
{
	return AssetId.IsValid();
}

void UAssetUI::SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{

}

void UAssetUI::SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{

}

void UAssetUI::RefreshDetails()
{

}

void UAssetUI::ResetDetails()
{

}

void UAssetUI::CancelInitialization()
{
	if (LatentId.IsValid() && IsValid(AssetManager))
	{
		AssetManager->CancelFetch(LatentId);
	}
}

void UAssetUI::SwitchDetails(bool bPrimary)
{

}

void UAssetUI::CloseWidget()
{
	RemoveFromParent();
}

void UAssetUI::NativeConstruct()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	Super::NativeConstruct();
}

void UAssetUI::NativeDestruct()
{
	CancelInitialization();

	AssetManager = nullptr;

	Super::NativeDestruct();
}

