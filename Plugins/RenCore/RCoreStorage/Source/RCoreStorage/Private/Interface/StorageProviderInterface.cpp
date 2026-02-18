// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Interface/StorageProviderInterface.h"

// Engine Headers

// Project Headers
#include "Util/SubsystemUtil.h"



IStorageProviderInterface* IStorageProviderInterface::Get(UGameInstance* GameInstance)
{
	return SubsystemUtil::GetSubsystemInterface<IStorageProviderInterface>(GameInstance);
}

