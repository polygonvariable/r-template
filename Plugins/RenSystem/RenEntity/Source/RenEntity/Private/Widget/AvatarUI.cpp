// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarUI.h"

// Engine Headers

// Project Headers
#include "RCoreEntity/Public/AvatarAsset.h"
#include "RCoreAssetManager/Private/RAssetManager.inl"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEntity/Public/Widget/AvatarEntry.h"
#include "RenEntity/Public/EntityPrimaryAsset.h"



void UAvatarUI::InitializeDetails(const UAvatarEntry* Entry)
{
	LatentId = FGuid::NewGuid();

	if (!IsValid(Entry))
	{
		LOG_ERROR(LogAvatar, TEXT("Entry is invalid"));
		return;
	}

	const FPrimaryAssetId& AssetId = Entry->AvatarId;
	FAvatarRecord Record = Entry->Record;

	if (!EntityPrimaryAsset::IsValid(AssetId) || !LatentId.IsValid() || !IsValid(AssetManager))
	{
		LOG_ERROR(LogAvatar, TEXT("AssetId, LatentId, AssetManager is invalid"));
		return;
	}

	TFuture<FLatentResultAsset<UAvatarAsset>> Future = AssetManager->FetchPrimaryAsset<UAvatarAsset>(LatentId, AssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogAvatar, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UAvatarUI> WeakThis(this);
	Future.Next([WeakThis, Record](const FLatentResultAsset<UAvatarAsset>& Result)
		{
			UAvatarUI* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				const UAvatarAsset* Asset = Result.GetAsset();
				This->SetPrimaryDetails(Asset);
				This->SetSecondaryDetails(Asset, Record);
			}
		}
	);
}

void UAvatarUI::InitializeDetails(const UAvatarAsset* Asset, const FAvatarRecord& Record)
{
	if (!IsValid(Asset))
	{
		LOG_ERROR(LogAvatar, TEXT("Asset is invalid"));
		return;
	}

	SetPrimaryDetails(Asset);
	SetSecondaryDetails(Asset, Record);
}

void UAvatarUI::SetPrimaryDetails(const UAvatarAsset* Asset)
{
}

void UAvatarUI::SetSecondaryDetails(const UAvatarAsset* Asset, const FAvatarRecord& Record)
{
}

void UAvatarUI::RefreshDetails()
{
}

void UAvatarUI::ResetDetails()
{
}

void UAvatarUI::CancelLatentFetch()
{
	if (LatentId.IsValid() && IsValid(AssetManager))
	{
		AssetManager->CancelFetch(LatentId);
	}
}

void UAvatarUI::CloseWidget()
{
	RemoveFromParent();
}

void UAvatarUI::NativeConstruct()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	Super::NativeConstruct();
}

void UAvatarUI::NativeDestruct()
{
	CancelLatentFetch();
	AssetManager = nullptr;

	Super::NativeDestruct();
}

