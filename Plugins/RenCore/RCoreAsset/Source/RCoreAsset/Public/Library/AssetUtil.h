// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define RCORE_API RCOREASSET_API

// Forward Declarations
class UGameInstance;
class IAssetInstanceCollectionProvider;



class FAssetUtil
{

public:

	RCORE_API static IAssetInstanceCollectionProvider* GetAssetInterchange(UWorld* Context, const FPrimaryAssetId& HandleAssetId);
	RCORE_API static IAssetInstanceCollectionProvider* GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetId& HandleAssetId);

	RCORE_API static IAssetInstanceCollectionProvider* GetAssetInterchange(UWorld* Context, const FPrimaryAssetType& HandleType);
	RCORE_API static IAssetInstanceCollectionProvider* GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetType& HandleType);

};



// Module Macros
#undef RCORE_API

