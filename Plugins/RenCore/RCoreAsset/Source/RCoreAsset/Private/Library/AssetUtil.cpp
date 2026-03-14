// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/AssetUtil.h"

// Engine Headers

// Project Headers
#include "Interface/IAssetInstance.h"
#include "Settings/AssetSettings.h"



IAssetInstanceCollectionProvider* FAssetUtil::GetAssetInterchange(UWorld* Context, const FPrimaryAssetId& HandleAssetId)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetAssetInterchange(Context->GetGameInstance(), HandleAssetId.PrimaryAssetType);
}

IAssetInstanceCollectionProvider* FAssetUtil::GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetId& HandleAssetId)
{
	return GetAssetInterchange(Context, HandleAssetId.PrimaryAssetType);
}


IAssetInstanceCollectionProvider* FAssetUtil::GetAssetInterchange(UWorld* Context, const FPrimaryAssetType& HandleType)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetAssetInterchange(Context->GetGameInstance(), HandleType);
}

IAssetInstanceCollectionProvider* FAssetUtil::GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetType& HandleType)
{
	const TArray<UGameInstanceSubsystem*>& Subsystems = Context->GetSubsystemArray<UGameInstanceSubsystem>();

	for (UGameInstanceSubsystem* Subsystem : Subsystems)
	{
		if (!IsValid(Subsystem) || !Subsystem->Implements<UAssetInstanceCollectionProvider>())
		{
			continue;
		}

		IAssetInstanceCollectionProvider* TransactionInterface = Cast<IAssetInstanceCollectionProvider>(Subsystem);
		if (!TransactionInterface)
		{
			continue;
		}

		if (TransactionInterface->GetSupportedAssetType() == HandleType)
		{
			return TransactionInterface;
		}
	}

	return nullptr;
}

