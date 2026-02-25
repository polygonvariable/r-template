// Copyright Epic Games, Inc. All Rights Reserved.

#include "RCoreTask.h"

#define LOCTEXT_NAMESPACE "FRCoreTaskModule"

void FRCoreTaskModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRCoreTaskModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRCoreTaskModule, RCoreTask)

