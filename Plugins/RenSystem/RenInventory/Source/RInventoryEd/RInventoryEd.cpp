// Copyright Epic Games, Inc. All Rights Reserved.

#include "RInventoryEd.h"
#include "HAL/IConsoleManager.h"
#include "Subsystem/InventorySubsystem.h"
#include "Definition/InventoryPrimaryAsset.h"

#define LOCTEXT_NAMESPACE "FRInventoryEdModule"



void FRInventoryEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	RegisterCommand();
}

void FRInventoryEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UnregisterCommand();
}

void FRInventoryEdModule::RegisterCommand()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Inventory.AddItem"),
		TEXT("Add an item to an inventory container, args=<ContainerId> <ItemId> <Quantity>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRInventoryEdModule::AddItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Inventory.RemoveItem"),
		TEXT("Remove an item from an inventory container, args=<ContainerId> <ItemGuid> <Quantity>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRInventoryEdModule::RemoveItem),
		ECVF_Cheat
	));
}

void FRInventoryEdModule::UnregisterCommand()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	for (IConsoleCommand* Command : Commands)
	{
		ConsoleManager.UnregisterConsoleObject(Command);
	}
}

UInventorySubsystem* FRInventoryEdModule::GetInventorySubsystem(UWorld* World)
{
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UInventorySubsystem>();
}

void FRInventoryEdModule::AddItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 2 || !IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid arguments"));
		return;
	}

	UInventorySubsystem* InventorySubsystem = GetInventorySubsystem(World);
	if (IsValid(InventorySubsystem))
	{
		FGuid InventoryId = UInventorySubsystem::GetStorageId();
		FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(FName(*Args[0]));
		int Quantity = FCString::Atoi(*Args[1]);

		InventorySubsystem->AddItem(InventoryId, AssetId, Quantity);
	}
}

void FRInventoryEdModule::RemoveItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 3 || !IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid arguments"));
		return;
	}

	//UInventoryStorageSubsystem* InventoryStorageSubsystem = GetInventoryStorageSubsystem(World);
	//UInventorySubsystem* InventorySubsystem = GetInventorySubsystem(World);
	//if (IsValid(InventorySubsystem))
	//{
	//	FName ContainerId = FName(*Args[0]);
	//	FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(FName(*Args[1]));
	//	int Quantity = FCString::Atoi(*Args[2]);

	//	//InventorySubsystem->RemoveItem(ContainerId, ItemGuid, Quantity);
	//}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRInventoryEdModule, RInventoryEd)

