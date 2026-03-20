// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Asset/TransactionalAsset.h"
#include "Interface/IAscensionProvider.h"

// Generated Headers
#include "CharacterAsset.generated.h"

// Module Macros
#define RSYSTEM_API RCHARACTER_API

// Forward Declarations
class UGameplayAbility;
class UAssetGroup;
class UAssetCollection;
class ARCharacterBase;

struct FAscensionData;



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UCharacterAsset : public UTransactionalAsset, public IAscensionProvider
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Health = 500;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Attack = 100;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Defense = 100;

	UPROPERTY(EditDefaultsOnly, Meta = (AssetBundles = "Character"))
	TSoftClassPtr<ARCharacterBase> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Meta = (AssetBundles = "Ability"))
	TArray<TSoftClassPtr<UGameplayAbility>> CharacterAbilities;


	// ~ IAscensionProviderInterface
	RSYSTEM_API virtual int GetExperienceInterval(int Level) const override;
	RSYSTEM_API virtual int GetLevelInterval(int Rank) const override;
	RSYSTEM_API virtual int GetMaxLevel() const override;
	RSYSTEM_API virtual int GetMaxRank() const override;
	RSYSTEM_API virtual const UAssetCollection* GetExperienceAssets(const FAscensionData& Ascension) const override;
	RSYSTEM_API virtual const UAssetCollection* GetRankAssets(const FAscensionData& Ascension) const override;
	// ~ End of IAscensionProviderInterface

	// ~ UPrimaryDataAsset
	RSYSTEM_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

protected:

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int ExperienceInterval = 5000;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int LevelInterval = 10;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int MaxLevel = 100;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int MaxRank = 100;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> LevelUpAssets = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> RankUpAssets = nullptr;

public:

	RSYSTEM_API static FPrimaryAssetType GetPrimaryAssetType();

};


// Module Macros
#undef RSYSTEM_API

