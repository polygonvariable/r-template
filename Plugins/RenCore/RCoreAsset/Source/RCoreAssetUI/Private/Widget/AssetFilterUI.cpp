// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetFilterUI.h"

// Engine Headers

// Project Headers



void UAssetFilterUI::SwitchDetail(bool bPrimary)
{
	ESlateVisibility NewVisibility = (bPrimary) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	SetVisibility(NewVisibility);
}



//UAssetFilter2UI::UAssetFilter2UI(const FObjectInitializer& ObjectInitializer)
//{
//}
//TSharedRef<SWidget> UAssetFilter2UI::RebuildWidget()
//{
//    BasePanel = SNew(SVerticalBox);
//
//    for (UPanelSlot* InSlot : Slots)
//    {
//        if (InSlot && InSlot->Content)
//        {
//            BasePanel->AddSlot()
//                [
//                    InSlot->Content->TakeWidget()
//                ];
//        }
//    }
//
//    return BasePanel.ToSharedRef();
//}
//
//void UAssetFilter2UI::OnSlotAdded(UPanelSlot* InSlot)
//{
//    if (BasePanel.IsValid() && InSlot && Slot->Content)
//    {
//        BasePanel->AddSlot()
//            [
//                InSlot->Content->TakeWidget()
//            ];
//    }
//}
//
//void UAssetFilter2UI::OnSlotRemoved(UPanelSlot* InSlot)
//{
//    if (BasePanel.IsValid())
//    {
//        BasePanel->ClearChildren();
//
//        for (UPanelSlot* PanelSlot : Slots)
//        {
//            if (PanelSlot && PanelSlot->Content)
//            {
//                BasePanel->AddSlot()
//                    [
//                        PanelSlot->Content->TakeWidget()
//                    ];
//            }
//        }
//    }
//}
//
//void UAssetFilter2UI::ReleaseSlateResources(bool bReleaseChildren)
//{
//    Super::ReleaseSlateResources(bReleaseChildren);
//    BasePanel.Reset();
//}