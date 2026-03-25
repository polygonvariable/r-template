// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/AssetInstanceUtil.h"

// Project Headers
#include "Interface/IAssetInstanceCollectionProvider.h"
#include "Settings/AssetInstanceSettings.h"



IAssetInstanceCollectionProvider* FAssetInstanceUtil::GetAssetInterchange(UWorld* Context, const FPrimaryAssetId& HandleAssetId)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetAssetInterchange(Context->GetGameInstance(), HandleAssetId.PrimaryAssetType);
}

IAssetInstanceCollectionProvider* FAssetInstanceUtil::GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetId& HandleAssetId)
{
	return GetAssetInterchange(Context, HandleAssetId.PrimaryAssetType);
}


IAssetInstanceCollectionProvider* FAssetInstanceUtil::GetAssetInterchange(UWorld* Context, const FPrimaryAssetType& HandleType)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetAssetInterchange(Context->GetGameInstance(), HandleType);
}

IAssetInstanceCollectionProvider* FAssetInstanceUtil::GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetType& HandleType)
{
	const TArray<UGameInstanceSubsystem*>& Subsystems = Context->GetSubsystemArray<UGameInstanceSubsystem>();

	for (UGameInstanceSubsystem* Subsystem : Subsystems)
	{
		IAssetInstanceCollectionProvider* TransactionInterface = Cast<IAssetInstanceCollectionProvider>(Subsystem);
		if (TransactionInterface)
		{
			if (TransactionInterface->GetSupportedAssetType() == HandleType)
			{
				return TransactionInterface;
			}
		}
	}

	return nullptr;
}

