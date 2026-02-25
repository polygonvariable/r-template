// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Filter/FilterCriterion.h"

// Generated Headers
#include "FilterLeafCriterion.generated.h"

// Module Macros
#define RCORE_API RCOREFILTER_API

// Forward Declarations



/**
 *
 */
UCLASS(Abstract)
class UFilterLeafCriterion : public UFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PropertyName;

	virtual FName GetPropertyName() const override;
	virtual bool GetIsLeaf() const override;

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Text)")
class UFilterTextCriterion : public UFilterLeafCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Included;

	RCORE_API virtual bool Evaluate(const FFilterContext& Context) const override;

};


/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Guid)")
class UFilterGuidCriterion : public UFilterLeafCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGuid> Included;

	RCORE_API virtual bool Evaluate(const FFilterContext& Context) const override;

};


/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Asset)")
class UFilterAssetCriterion : public UFilterLeafCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPrimaryAssetId> Included;

	RCORE_API virtual bool Evaluate(const FFilterContext& Context) const override;

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Integer)")
class UFilterIntegerCriterion : public UFilterLeafCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Min = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Max = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableStrictMode = true;

	RCORE_API virtual bool Evaluate(const FFilterContext& Context) const override;

};



// Module Macros
#undef RCORE_API

