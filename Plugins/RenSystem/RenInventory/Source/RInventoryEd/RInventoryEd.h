// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Forward declaration
class UInventorySubsystem;



class FRInventoryEdModule : public IModuleInterface
{

public:

	// ~ IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// ~ End of IModuleInterface

protected:

	TArray<IConsoleCommand*> Commands;

	void RegisterCommand();
	void UnregisterCommand();

	void AddItem(const TArray<FString>& Args, UWorld* World);
	void RemoveItem(const TArray<FString>& Args, UWorld* World);
	void GetItem(const TArray<FString>& Args, UWorld* World);

};

