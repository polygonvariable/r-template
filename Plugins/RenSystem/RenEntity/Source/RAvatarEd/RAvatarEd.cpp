// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "RAvatarEd.h"

// Engine Headers
#include "HAL/IConsoleManager.h"

// Project Headers
#include "Library/EntityPrimaryAsset.h"
#include "Subsystem/AvatarSubsystem.h"
#include "Storage/AvatarStorage.h"

#define LOCTEXT_NAMESPACE "FRAvatarEdModule"



void FRAvatarEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterCommand();
}

void FRAvatarEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UnregisterCommand();
}

void FRAvatarEdModule::RegisterCommand()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Avatar.AddItem"),
		TEXT("Add avatar, args=<CollectionId> <AssetId>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRAvatarEdModule::AddItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Avatar.RemoveItem"),
		TEXT("Remove avatar, args=<CollectionId> <AssetId>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRAvatarEdModule::RemoveItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Avatar.GetItem"),
		TEXT("Prints details of avatar, args=<CollectionId> <AssetId>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRAvatarEdModule::GetItem),
		ECVF_Cheat
	));
}

void FRAvatarEdModule::UnregisterCommand()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	for (IConsoleCommand* Command : Commands)
	{
		ConsoleManager.UnregisterConsoleObject(Command);
	}
}

UAvatarStorage* FRAvatarEdModule::GetAvatarCollection(UWorld* World, FName CollectionId)
{
	UAvatarSubsystem* Subsystem = UAvatarSubsystem::Get(World);
	if (!IsValid(Subsystem))
	{
		return nullptr;
	}
	return Subsystem->GetAvatarCollection(CollectionId);
}

void FRAvatarEdModule::AddItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 2 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UAvatarStorage* Collection = GetAvatarCollection(World, FName(*Args[0]));
	if (IsValid(Collection))
	{
		FPrimaryAssetId AssetId = FEntityPrimaryAsset::GetPrimaryAssetId(FName(*Args[1]));
		Collection->AddItem(AssetId, 1);
	}
}

void FRAvatarEdModule::RemoveItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 2 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UAvatarStorage* Collection = GetAvatarCollection(World, FName(*Args[0]));
	if (IsValid(Collection))
	{
		FPrimaryAssetId AssetId = FEntityPrimaryAsset::GetPrimaryAssetId(FName(*Args[1]));
		Collection->RemoveItem(AssetId, 1);
	}
}

void FRAvatarEdModule::GetItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 2 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UAvatarStorage* Collection = GetAvatarCollection(World, FName(*Args[0]));
	if (IsValid(Collection))
	{
		FPrimaryAssetId AssetId = FEntityPrimaryAsset::GetPrimaryAssetId(FName(*Args[1]));
		const FAvatarData* Item = Collection->GetItem(AssetId);
		if (!Item)
		{
			UE_LOG(LogTemp, Error, TEXT("Avatar not found"));
			return;
		}

		FString ItemDebug = Item->ToString();
		UE_LOG(LogTemp, Log, TEXT("%s"), *ItemDebug);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRAvatarEdModule, RAvatarEd)

