// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CatalogUI.h"

// Engine Headers

// Project Headers
#include "RCoreAssetManager/Private/RAssetManager.inl"
#include "RCoreAssetManager/Public/Widget/CatalogEntry.h"

#include "Log/LogCategory.h"
#include "Log/LogMacro.h"




void UCatalogUI::SetCatalogId(const FGuid& Id)
{
	CatalogId = Id;
}

void UCatalogUI::InitializeDetails(const UCatalogEntry* Entry)
{
	LatentId = FGuid::NewGuid();

	if (!IsValid(Entry))
	{
		LOG_ERROR(LogAvatar, TEXT("Entry is invalid"));
		return;
	}

	ActiveAssetId = Entry->AssetId;
	if (!IsPrimaryAssetIdValid(ActiveAssetId) || !LatentId.IsValid() || !IsValid(AssetManager))
	{
		LOG_ERROR(LogAvatar, TEXT("AssetId, LatentId, AssetManager is invalid"));
		return;
	}

	TFuture<FLatentResultAsset<UPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAsset<UPrimaryDataAsset>(LatentId, ActiveAssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogAvatar, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<const UCatalogEntry> WeakEntry(Entry);
	TWeakObjectPtr<UCatalogUI> WeakThis(this);

	Future.Next([WeakThis, WeakEntry](const FLatentResultAsset<UPrimaryDataAsset>& Result)
		{
			UCatalogUI* This = WeakThis.Get();
			const UCatalogEntry* Entry = WeakEntry.Get();
			if (IsValid(This) && Result.IsValid())
			{
				const UPrimaryDataAsset* Asset = Result.GetAsset();
				This->SetPrimaryDetails(Entry, Asset);
				This->SetSecondaryDetails(Entry, Asset);
			}
		}
	);
}

void UCatalogUI::InitializeDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	if (IsValid(Asset))
	{
		ActiveAssetId = Asset->GetPrimaryAssetId();
	}

	SetPrimaryDetails(Entry, Asset);
	SetSecondaryDetails(Entry, Asset);
}

bool UCatalogUI::IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const
{
	return AssetId.IsValid();
}

void UCatalogUI::SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
}

void UCatalogUI::SetSecondaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
}

void UCatalogUI::RefreshDetails()
{
}

void UCatalogUI::ResetDetails()
{
}

void UCatalogUI::CancelAssetLoading()
{
	if (LatentId.IsValid() && IsValid(AssetManager))
	{
		AssetManager->CancelFetch(LatentId);
	}
}

void UCatalogUI::SwitchDetails(bool bPrimary)
{

}

void UCatalogUI::CloseWidget()
{
	RemoveFromParent();
}

void UCatalogUI::NativeConstruct()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	Super::NativeConstruct();
}

void UCatalogUI::NativeDestruct()
{
	CancelAssetLoading();

	AssetManager = nullptr;

	Super::NativeDestruct();
}

