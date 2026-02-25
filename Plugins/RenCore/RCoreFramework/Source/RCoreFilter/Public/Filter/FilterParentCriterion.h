// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/QueryType.h"
#include "Filter/FilterCriterion.h"

// Generated Headers
#include "FilterParentCriterion.generated.h"

// Module Macros
#define RCORE_API RCOREFILTER_API

// Forward Declarations
struct FFilterContext;



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (AND/OR)")
class UFilterGroupCriterion : public UFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFilterOperator Operator = EFilterOperator::And;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<TObjectPtr<UFilterCriterion>> Criteria;

	RCORE_API virtual bool Evaluate(const FFilterContext& Context) const override;

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (NOT)")
class UFilterNotCriterion : public UFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UFilterCriterion> Negate;

	RCORE_API virtual bool Evaluate(const FFilterContext& Context) const override;

};



// Module Macros
#undef RCORE_API

