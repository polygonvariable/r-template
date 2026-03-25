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
UCLASS(MinimalAPI)
class UPartyStorage : public UStorage
{

	GENERATED_BODY()

public:

	RSYSTEM_API FVector GetPartyLocation(const FName& Level) const;
	RSYSTEM_API void SetPartyLocation(const FName& Level, const FVector& Location);

	RSYSTEM_API const TArray<FPrimaryAssetId>& GetCharacters() const;
	RSYSTEM_API const TArray<FPrimaryAssetId>& GetTemporaryCharacters() const;
	RSYSTEM_API void GetAllCharacters(TArray<FPrimaryAssetId>& OutCharacters) const;

	RSYSTEM_API FPrimaryAssetId GetCharacterAtSlot(int Slot) const;
	RSYSTEM_API bool SetCharacterAtSlot(int Slot, FPrimaryAssetId AssetId);
	RSYSTEM_API bool ClearSlot(int Slot);

	// ~ UStorage
	RSYSTEM_API virtual void InitializeDefaults() override;
	// ~ End of UStorage

protected:

	/* Party Locations on different levels */
	UPROPERTY(SaveGame)
	TMap<FName, FVector> PartyLocations;

	UPROPERTY(SaveGame)
	TArray<FPrimaryAssetId> CharacterSlot;

	UPROPERTY(Transient)
	TArray<FPrimaryAssetId> TemporarySlot;

};


// Module Macros
#undef RSYSTEM_API

