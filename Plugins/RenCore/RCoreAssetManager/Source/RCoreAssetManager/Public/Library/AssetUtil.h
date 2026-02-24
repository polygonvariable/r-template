// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define RCORE_API RCOREASSETMANAGER_API

// Forward Declarations
class UGameInstance;
class IAssetTransactionInterface;



class AssetUtil
{

public:

	RCORE_API static IAssetTransactionInterface* GetTransactionInterface(UGameInstance* Context, const FPrimaryAssetType& HandleType);

};



// Module Macros
#undef RCORE_API

