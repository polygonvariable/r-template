// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define RCORE_API RCOREASSET_API

// Forward Declarations
class UGameInstance;
class IAssetInterchangeInterface;



class AssetUtil
{

public:

	RCORE_API static IAssetInterchangeInterface* GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetId& HandleType);
	RCORE_API static IAssetInterchangeInterface* GetAssetInterchange(UGameInstance* Context, const FPrimaryAssetType& HandleType);

};



// Module Macros
#undef RCORE_API

