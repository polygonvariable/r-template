// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarDetailUI.h"

// Engine Headers
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreEntity/Public/AvatarAsset.h"
#include "RenEntity/Public/Widget/AvatarEntry.h"
#include "RenEntity/Public/EntityPrimaryAsset.h"



void UAvatarDetailUI::RefreshDetails()
{

}

void UAvatarDetailUI::ResetDetails()
{

}

void UAvatarDetailUI::SetPrimaryDetails(const UAvatarAsset* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}

	if (EntryName) EntryName->SetText(Asset->DisplayName);
	if (EntryDescription) EntryDescription->SetText(Asset->Description);
	if (EntryIcon) EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
}

void UAvatarDetailUI::SetSecondaryDetails(const UAvatarAsset* Asset, const FAvatarRecord& Record)
{
	const FEnhanceRecord& Enhance = Record.Enhance;
	if (EntryRank) EntryRank->SetText(FText::FromString(FString::FromInt(Enhance.Rank)));
	if (EntryLevel) EntryLevel->SetText(FText::FromString(FString::FromInt(Enhance.Level)));
	if (EntryExperience) EntryExperience->SetText(FText::FromString(FString::FromInt(Enhance.Experience)));
}

void UAvatarDetailUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAvatarDetailUI::NativeDestruct()
{
	Super::NativeDestruct();
}

