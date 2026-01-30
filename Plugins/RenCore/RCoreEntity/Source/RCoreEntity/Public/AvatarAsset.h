// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers

// Generated Headers
#include "AvatarAsset.generated.h"

// Forward Declarations




/*
USTRUCT(BlueprintType)
struct FExchangeData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetId CostItem;

	UPROPERTY(EditDefaultsOnly)
	int CostAmount = 0;

	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetId ResultItem;

	UPROPERTY(EditDefaultsOnly)
	int ResultAmount = 0;

};
*/

/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UAvatarAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	int Health = 500;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	int Attack = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	int Defense = 100;


	RCOREENTITY_API static FPrimaryAssetType GetPrimaryAssetType();
	RCOREENTITY_API static FPrimaryAssetId MakePrimaryAssetId(const FName& AssetName);

	// ~ UPrimaryDataAsset
	RCOREENTITY_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

};

