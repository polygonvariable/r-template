// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "SaveGame/Storage.h"

// Generated Headers
#include "PartyStorage.generated.h"

// Module Macros
#define RSYSTEM_API RCHARACTERPARTY_API



/**
 *
 */
struct FPartyMemberInfo
{
public:

	FPartyMemberInfo() {}
	FPartyMemberInfo(const FPrimaryAssetId& InAssetId, bool bInTemporary = false) : AssetId(InAssetId), bTemporary(bInTemporary) {}

	FPrimaryAssetId AssetId;
	bool bTemporary = false;
};



/**
 *
 */
UCLASS(MinimalAPI)
class UPartyStorage : public UStorage
{

	GENERATED_BODY()

public:

	RSYSTEM_API bool AddCharacter(const FPrimaryAssetId& AssetId, bool bTemporary = false);
	RSYSTEM_API void RemoveCharacter(const FPrimaryAssetId& AssetId);
	RSYSTEM_API bool HasCharacter(const FPrimaryAssetId& AssetId) const;

	RSYSTEM_API FVector GetPartyLocation(const FName& Level) const;
	RSYSTEM_API void SetPartyLocation(const FName& Level, const FVector& Location);

	RSYSTEM_API const TArray<FPrimaryAssetId>& GetCharacters() const;
	RSYSTEM_API const TArray<FPrimaryAssetId>& GetTemporaryCharacters() const;
	RSYSTEM_API void GetAllCharacters(TArray<FPrimaryAssetId>& OutCharacters) const;

	// ~ UStorage
	RSYSTEM_API virtual void InitializeDefaults() override;
	// ~ End of UStorage

protected:

	UPROPERTY(SaveGame)
	TMap<FName, FVector> PartyLocations; // Party Locations on different levels

	UPROPERTY(SaveGame)
	TArray<FPrimaryAssetId> CharacterList;

	UPROPERTY(Transient)
	TArray<FPrimaryAssetId> TemporaryList;

};


// Module Macros
#undef RSYSTEM_API

