// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Delegate/LatentDelegate.h"

// Engine Headers

// Project Headers


FLatentDelegate::FLatentTaskDelegate FLatentDelegate::OnStorageLoaded;
FLatentDelegate::FLatentTaskDelegate FLatentDelegate::OnPreGameInitialized;
FLatentDelegate::FLatentTaskDelegate FLatentDelegate::OnPostGameInitialized;

