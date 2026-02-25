// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/AssetUtil.h"

// Engine Headers

// Project Headers
#include "Interface/AssetTransactionInterface.h"



IAssetTransactionInterface* AssetUtil::GetTransactionInterface(UGameInstance* Context, const FPrimaryAssetType& HandleType)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}

	const TArray<UGameInstanceSubsystem*>& Subsystems = Context->GetSubsystemArray<UGameInstanceSubsystem>();

	for (UGameInstanceSubsystem* Subsystem : Subsystems)
	{
		if (!IsValid(Subsystem) || !Subsystem->Implements<UAssetTransactionInterface>())
		{
			continue;
		}

		IAssetTransactionInterface* TransactionInterface = Cast<IAssetTransactionInterface>(Subsystem);
		if (!TransactionInterface)
		{
			continue;
		}

		if (TransactionInterface->GetHandledAssetType() == HandleType)
		{
			return TransactionInterface;
		}
	}

	return nullptr;
}

