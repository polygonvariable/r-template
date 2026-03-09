// Copyright Epic Games, Inc. All Rights Reserved.

#include "RInventoryEd.h"
#include "HAL/IConsoleManager.h"
#include "Subsystem/InventorySubsystem.h"
#include "Storage/InventoryStorage.h"
#include "Library/InventoryPrimaryAsset.h"

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
		TEXT("Add an item to an inventory container, args=<AssetId> <Quantity>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRInventoryEdModule::AddItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Inventory.RemoveItem"),
		TEXT("Remove an item from an inventory container, args=<AssetId> <Quantity>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRInventoryEdModule::RemoveItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Inventory.GetItem"),
		TEXT("Prints details of an item, args=<AssetId>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRInventoryEdModule::GetItem),
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

void FRInventoryEdModule::AddItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 2 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(World);
	if (!IsValid(InventorySubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid inventory subsystem"));
		return;
	}

	UInventoryStorage* Inventory = InventorySubsystem->GetInventory(UInventorySubsystem::GetStorageId());
	if (!IsValid(Inventory))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid inventory"));
		return;
	}

	FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(FName(*Args[0]));
	int Quantity = FCString::Atoi(*Args[1]);

	Inventory->AddItem(AssetId, Quantity);
}

void FRInventoryEdModule::RemoveItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 2 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(World);
	if (!IsValid(InventorySubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid inventory subsystem"));
		return;
	}

	UInventoryStorage* Inventory = InventorySubsystem->GetInventory(UInventorySubsystem::GetStorageId());
	if (!IsValid(Inventory))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid inventory"));
		return;
	}

	FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(FName(*Args[0]));
	int Quantity = FCString::Atoi(*Args[1]);

	Inventory->RemoveItem(AssetId, Quantity);
}

void FRInventoryEdModule::GetItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 1 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(World);
	if (!IsValid(InventorySubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid inventory subsystem"));
		return;
	}

	UInventoryStorage* Inventory = InventorySubsystem->GetInventory(UInventorySubsystem::GetStorageId());
	if (!IsValid(Inventory))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid inventory"));
		return;
	}

	FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(FName(*Args[0]));
	const FInventoryItem* Item = Inventory->GetItem(AssetId);
	if (!Item)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid item"));
		return;
	}

	FString ItemDetail = Item->ToString();

	UE_LOG(LogTemp, Log, TEXT("%s"), *ItemDetail);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRInventoryEdModule, RInventoryEd)

