// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryBase.generated.h"

/**
 * 
 */
UCLASS()
class SYSTEMS_API UInventoryBase : public UObject
{
	GENERATED_BODY()
	

public:
	// functions and stufferino
	virtual void CreateInventory();

public:
	// variables and stufferino
	int8 InventorySize = 10;
};
