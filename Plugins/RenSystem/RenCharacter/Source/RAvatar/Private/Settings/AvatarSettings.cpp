// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Settings/AvatarSettings.h"

// Project Headers
#include "Storage/AvatarStorage.h"



UAvatarSettings::UAvatarSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");
}

const FName& UAvatarSettings::GetStorageId() const
{
	return StorageId;
}

TSubclassOf<UStorage> UAvatarSettings::GetStorageClass() const
{
	return StorageClass;
}

const UAvatarSettings* UAvatarSettings::Get()
{
	return GetDefault<UAvatarSettings>();
}

