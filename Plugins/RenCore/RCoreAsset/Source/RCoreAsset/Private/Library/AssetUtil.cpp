// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/AssetUtil.h"

// Engine Headers

// Project Headers
#include "Interface/AssetTransactionInterface.h"



IAssetInterchangeInterface* FAssetUtil::GetAssetInterchange(UWorld* Context, const FPrimaryAssetId& HandleAssetId)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetAssetInterchange(Context->GetGameInstance(), HandleAssetId.PrimaryAssetType);
}

IAssetInterchangeInterface* FAssetUtil::GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetId& HandleAssetId)
{
	return GetAssetInterchange(Context, HandleAssetId.PrimaryAssetType);
}


IAssetInterchangeInterface* FAssetUtil::GetAssetInterchange(UWorld* Context, const FPrimaryAssetType& HandleType)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetAssetInterchange(Context->GetGameInstance(), HandleType);
}

IAssetInterchangeInterface* FAssetUtil::GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetType& HandleType)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}

	const TArray<UGameInstanceSubsystem*>& Subsystems = Context->GetSubsystemArray<UGameInstanceSubsystem>();

	for (UGameInstanceSubsystem* Subsystem : Subsystems)
	{
		if (!IsValid(Subsystem) || !Subsystem->Implements<UAssetInterchangeInterface>())
		{
			continue;
		}

		IAssetInterchangeInterface* TransactionInterface = Cast<IAssetInterchangeInterface>(Subsystem);
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

