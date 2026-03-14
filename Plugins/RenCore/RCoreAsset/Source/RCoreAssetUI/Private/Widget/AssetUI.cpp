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



void UAssetUI::InitializeDetail()
{

}

void UAssetUI::InitializeDetail(const UAssetEntry* Entry)
{
	if (!IsValid(Entry) || !IsValid(AssetManager) || !Entry->AssetId.IsValid())
	{
		LOG_ERROR(LogAsset, TEXT("Entry, AssetManager or AssetId is invalid"));
		return;
	}

	if (_ActiveAssetId == Entry->AssetId && _ActiveAsset.IsValid())
	{
		const URPrimaryDataAsset* Asset = _ActiveAsset.Get();
		SetPrimaryDetail(Entry, Asset);
		SetSecondaryDetail(Entry, Asset);
		return;
	}

	AssetManager->CancelFetch(_ActiveLoadId);

	_ActiveAssetId = Entry->AssetId;
	_ActiveLoadId = FGuid::NewGuid();

	TFuture<FLatentLoadedAsset<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAsset<URPrimaryDataAsset>(_ActiveLoadId, _ActiveAssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogAsset, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<const UAssetEntry> WeakEntry(Entry);
	TWeakObjectPtr<UAssetUI> WeakThis(this);

	Future.Next([WeakThis, WeakEntry](const FLatentLoadedAsset<URPrimaryDataAsset>& Result)
		{
			UAssetUI* This = WeakThis.Get();
			const UAssetEntry* Entry = WeakEntry.Get();
			if (IsValid(This) && Result.IsValid())
			{
				const URPrimaryDataAsset* Asset = Result.Get();

				This->_ActiveAsset = TWeakObjectPtr<const URPrimaryDataAsset>(Asset);

				This->SetPrimaryDetail(Entry, Asset);
				This->SetSecondaryDetail(Entry, Asset);
			}
		}
	);
}

void UAssetUI::InitializeDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	if (IsValid(Asset))
	{
		_ActiveAssetId = Asset->GetPrimaryAssetId();
	}

	SetPrimaryDetail(Entry, Asset);
	SetSecondaryDetail(Entry, Asset);
}

const FPrimaryAssetId& UAssetUI::GetActiveAssetId() const
{
	return _ActiveAssetId;
}

void UAssetUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{

}

void UAssetUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{

}

void UAssetUI::RefreshDetail()
{

}

void UAssetUI::ResetDetail()
{

}

void UAssetUI::CancelInitialization()
{
	if (_ActiveLoadId.IsValid() && IsValid(AssetManager))
	{
		AssetManager->CancelFetch(_ActiveLoadId);
	}
}

void UAssetUI::SwitchDetail(bool bPrimary)
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

