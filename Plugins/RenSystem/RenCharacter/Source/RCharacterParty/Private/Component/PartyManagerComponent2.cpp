// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/PartyManagerComponent2.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "Actor/RCharacterBase.h"
#include "Asset/CharacterAsset.h"
#include "Asset/RPrimaryDataAsset.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Storage/PartyStorage.h"
#include "Storage/AvatarStorage.h"
#include "Subsystem/AvatarSubsystem.h"
#include "Subsystem/PartySubsystem.h"



UPartyManagerComponent2::UPartyManagerComponent2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UPartyManagerComponent2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UPartyManagerComponent2::BeginPlay()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	UPartySubsystem* PartySubsystem = UPartySubsystem::Get(GetWorld());
	if (IsValid(PartySubsystem))
	{
		PartyStorage = PartySubsystem->GetPartyCollection();
	}

	Super::BeginPlay();
}

void UPartyManagerComponent2::SpawnParty()
{
	UPartyStorage* Party = PartyStorage.Get();
	if (!IsValid(AssetManager) || !IsValid(Party))
	{
		LOG_ERROR(LogTemp, TEXT("AssetManager or Party Storage is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Characters;
	Party->GetAllCharacters(Characters);

	AssetManager->CancelFetch(_PartySpawnId);

	_PartySpawnId = FGuid::NewGuid();
	_PartySpawnLocation = Party->GetPartyLocation(*GetWorld()->GetMapName());

	TArray<FName> AssetBundles;
	AssetBundles.Add(TEXT("Character"));
	AssetBundles.Add(TEXT("Ability"));

	TFuture<FLatentLoadedAssets<UCharacterAsset>> Future = AssetManager->FetchPrimaryAssets<UCharacterAsset>(_PartySpawnId, Characters, AssetBundles);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("Failed to initialize load character assets"));
		return;
	}

	TWeakObjectPtr<UPartyManagerComponent2> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCharacterAsset>& Result)
		{
			UPartyManagerComponent2* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->SpawnParty_Internal(Result.Get());
			}
		}
	);
}

void UPartyManagerComponent2::SpawnParty_Internal(const TArray<UCharacterAsset*>& Assets)
{
	UWorld* World = GetWorld();

	for (const UCharacterAsset* Asset : Assets)
	{
		if (IsValid(Asset))
		{
			SpawnCharacter(World, Asset);
		}
	}

	APlayerController* PC = GetControllerWithAuthority();
	ACharacter* Character = GetAliveCharacter();
	if (IsValid(Character) && IsValid(PC))
	{
		PC->Possess(Character);
	}
}


void UPartyManagerComponent2::SpawnCharacter(UWorld* World, const UCharacterAsset* CharacterAsset)
{
	UClass* CharacterClass = CharacterAsset->CharacterClass.Get();
	if (!CharacterClass)
	{
		LOG_ERROR(LogTemp, TEXT("World, fragment is invalid"));
		return;
	}

	FTransform SpawnTransform;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ARCharacterBase* SpawnedAvatar = World->SpawnActorDeferred<ARCharacterBase>(CharacterClass, SpawnTransform);
	if (!IsValid(SpawnedAvatar))
	{
		LOG_ERROR(LogTemp, TEXT("Spawned avatar is invalid"));
		return;
	}

	SpawnedAvatar->InitializeCharacter(CharacterAsset);
	SpawnedAvatar->FinishSpawning(SpawnTransform);

	RegisterCharacter(SpawnedAvatar);
}

void UPartyManagerComponent2::RemoveCharacter(ARCharacterBase* Character)
{
	UnregisterCharacter(Character);

	Character->Destroy();
}


void UPartyManagerComponent2::RegisterCharacter(ARCharacterBase* Character)
{
	Character->OnCharacterDied.AddUObject(this, &UPartyManagerComponent2::HandleCharacterDied);
	PartyCharacters.Add(Character);
}

void UPartyManagerComponent2::UnregisterCharacter(ARCharacterBase* Character)
{
	Character->OnCharacterDied.RemoveAll(this);
	PartyCharacters.Remove(Character);
}


void UPartyManagerComponent2::HandleCharacterDied()
{
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(PC))
	{
		LOG_ERROR(LogTemp, TEXT("PC is invalid"));
		return;
	}

	ACharacter* Character = GetAliveCharacter();
	if (IsValid(Character))
	{
		PC->Possess(Character);
		return;
	}

	// all avatars are dead
	// revive all characters
}


void UPartyManagerComponent2::ClearParty()
{
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(PC))
	{
		LOG_ERROR(LogTemp, TEXT("PC is invalid"));
		return;
	}

	PC->UnPossess();
}

ACharacter* UPartyManagerComponent2::GetAliveCharacter() const
{
	for (const TWeakObjectPtr<ARCharacterBase>& Character : PartyCharacters)
	{
		ARCharacterBase* Actor = Character.Get();
		if (IsValid(Actor) && Actor->IsAlive())
		{
			return Actor;
		}
	}
	return nullptr;
}


APlayerController* UPartyManagerComponent2::GetControllerWithAuthority() const
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	const ENetRole OwnerRole = GetOwnerRole();
	if (OwnerRole != ENetRole::ROLE_Authority || !IsValid(PC))
	{
		return nullptr;
	}
	return PC;
}