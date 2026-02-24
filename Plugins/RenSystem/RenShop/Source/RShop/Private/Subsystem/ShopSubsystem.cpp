// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/ShopSubsystem.h"

// Engine Headers

// Project Headers
#include "LatentDelegates.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/PurchaseItem.h"



void UShopSubsystem::PurchaseItem(const FGuid& TaskId, const FPrimaryAssetId& ShopAssetId, const FPrimaryAssetId& ItemAssetId, const FPrimaryAssetId& CostAssetId, FTaskCallback Callback)
{
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogItemAscension, TEXT("Task subsystem is invalid"));
		return;
	}

	UPurchaseItem* Task = TaskSubsystem->CreateTask<UPurchaseItem>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogItemAscension, TEXT("Failed to create task"));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->ShopAssetId = ShopAssetId;
	Task->ItemAssetId = ItemAssetId;
	Task->CostAssetId = CostAssetId;
	Task->StartTask();
}

void UShopSubsystem::OnPreGameInitialized()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
}

bool UShopSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UShopSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UShopSubsystem::OnPreGameInitialized);

	LOG_WARNING(LogItemShop, TEXT("ItemShopSubsystem initialized"));
}

void UShopSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	TaskSubsystem = nullptr;

	LOG_WARNING(LogItemShop, TEXT("ItemShopSubsystem deinitialized"));
	Super::Deinitialize();
}



UShopSubsystem* UShopSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UShopSubsystem* UShopSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UShopSubsystem>();
}

