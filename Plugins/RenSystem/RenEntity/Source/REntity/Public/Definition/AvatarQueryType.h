// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AssetQuerySource.h"
#include "Definition/AvatarSortType.h"
#include "Definition/QueryType.h"

// Generated Headers
#include "AvatarQueryType.generated.h"



/**
 *
 * 
 */
USTRUCT(BlueprintType)
struct FAvatarQueryRule
{

	GENERATED_BODY()

public:

	FAvatarQueryRule() {}


	UPROPERTY(EditAnywhere)
	EAssetQuerySource QuerySource = EAssetQuerySource::Instance;

	UPROPERTY(EditAnywhere)
	ESortDirection SortDirection = ESortDirection::Ascending;

	UPROPERTY(EditAnywhere)
	EAvatarSortType SortType = EAvatarSortType::Alphabetical;

};

