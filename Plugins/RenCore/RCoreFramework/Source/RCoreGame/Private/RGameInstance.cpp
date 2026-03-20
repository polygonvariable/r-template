// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "RGameInstance.h"

// Project Headers
#include "Delegate/GameLifecycleDelegates.h"



void URGameInstance::Init()
{
	Super::Init();
	GameInit();
}

void URGameInstance::GameInit()
{
	FGameLifecycleDelegates::OnPreGameInitialized.Broadcast();
}

