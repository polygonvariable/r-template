// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Management/AssetCollection.h"

// Engine Headers

// Project Headers



const FGameplayTagContainer& UAssetCollection::GetCollectionTags() const
{
	return Tags;
}

