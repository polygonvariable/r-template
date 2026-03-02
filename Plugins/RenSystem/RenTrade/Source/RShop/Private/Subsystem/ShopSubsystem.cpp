// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/ShopSubsystem.h"

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/PurchaseItem.h"



void UShopSubsystem::PurchaseItem(const FGuid& TaskId, const FPrimaryAssetId& ShopAssetId, const FPrimaryAssetId& TargetAssetId, const FGameplayTagContainer& CostTags, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogShop, TEXT("Task subsystem is invalid"));
		return;
	}

	UPurchaseItem* Task = TaskSubsystem->CreateTask<UPurchaseItem>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogShop, TEXT("Failed to create task"));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->ShopAssetId = ShopAssetId;
	Task->TargetAssetId = TargetAssetId;
	Task->CostTags = CostTags;
	Task->StartTask();
}

bool UShopSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UShopSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogShop, TEXT("ShopSubsystem initialized"));
}

void UShopSubsystem::Deinitialize()
{
	LOG_WARNING(LogShop, TEXT("ShopSubsystem deinitialized"));
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

