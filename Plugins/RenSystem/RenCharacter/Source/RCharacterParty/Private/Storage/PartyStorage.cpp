// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/PartyStorage.h"

// Project Headers
#include "Settings/PartySettings.h"



void UPartyStorage::InitializeDefaults()
{
	CharacterList = UPartySettings::Get()->DefaultCharacters;
}

bool UPartyStorage::AddCharacter(const FPrimaryAssetId& AssetId, bool bTemporary)
{
	if (CharacterList.Contains(AssetId) || TemporaryList.Contains(AssetId))
	{
		return false;
	}

	if (bTemporary)
	{
		TemporaryList.Add(AssetId);
	}
	else
	{
		CharacterList.Add(AssetId);
	}

	OnStorageUpdated.Broadcast();
	return true;
}

void UPartyStorage::RemoveCharacter(const FPrimaryAssetId& AssetId)
{
	CharacterList.Remove(AssetId);
	TemporaryList.Remove(AssetId);

	OnStorageUpdated.Broadcast();
}

bool UPartyStorage::HasCharacter(const FPrimaryAssetId& AssetId) const
{
    return (CharacterList.Contains(AssetId) || TemporaryList.Contains(AssetId));
}

FVector UPartyStorage::GetPartyLocation(const FName& Level) const
{
    return PartyLocations.FindRef(Level);
}

void UPartyStorage::SetPartyLocation(const FName& Level, const FVector& Location)
{
    PartyLocations.Add(Level, Location);
}

const TArray<FPrimaryAssetId>& UPartyStorage::GetCharacters() const
{
    return CharacterList;
}

const TArray<FPrimaryAssetId>& UPartyStorage::GetTemporaryCharacters() const
{
    return TemporaryList;
}

void UPartyStorage::GetAllCharacters(TArray<FPrimaryAssetId>& OutCharacters) const
{
	OutCharacters.Append(CharacterList);
	OutCharacters.Append(TemporaryList);
}

