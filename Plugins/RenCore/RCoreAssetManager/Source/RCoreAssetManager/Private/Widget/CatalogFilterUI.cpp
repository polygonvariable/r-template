// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CatalogFilterUI.h"

// Engine Headers

// Project Headers



void UCatalogFilterUI::SwitchDetails(bool bPrimary)
{
	ESlateVisibility NewVisibility = (bPrimary) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	SetVisibility(NewVisibility);
}
