// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Category/EquipmentAsset.h"



const TSoftClassPtr<AActor>& UEquipmentAsset::GetEquipmentClass() const
{
	return EquipmentClass;
}

const TArray<FSoftClassPath>& UEquipmentAsset::GetEquipmentAbilities() const
{
	return EquipmentAbilities;
}

const FName& UEquipmentAsset::GetEquipmentSocket() const
{
	return SocketName;
}

