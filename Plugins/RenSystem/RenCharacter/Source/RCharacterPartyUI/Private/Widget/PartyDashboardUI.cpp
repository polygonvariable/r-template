// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/PartyDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Settings/AvatarSettings.h"
#include "Subsystem/PartySubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"



void UPartyDashboardUI::InitializeDetail()
{
	AvatarCollection->PrimarySourceId = PrimarySourceId;
	AvatarCollection->InitializeCollection();
	AvatarCollection->DisplayEntries();
}

void UPartyDashboardUI::ConfirmParty()
{
	UPartySubsystem* PartySubsystem = UPartySubsystem::Get(GetWorld());
	if(IsValid(PartySubsystem))
	{
		PartySubsystem->RequestSpawnParty();
	}
}

void UPartyDashboardUI::NativeConstruct()
{
	SpawnButton->OnClicked.AddDynamic(this, &UPartyDashboardUI::ConfirmParty);

	Super::NativeConstruct();
}

void UPartyDashboardUI::NativeDestruct()
{
	SpawnButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

