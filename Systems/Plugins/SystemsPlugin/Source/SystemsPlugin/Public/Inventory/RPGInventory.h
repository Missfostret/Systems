// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryBase.h"
#include "RPGInventory.generated.h"

/**
 * 
 */
UCLASS()
class SYSTEMSPLUGIN_API URPGInventory : public UInventoryBase
{
	GENERATED_BODY()
	
	virtual void CreateInventory() override;
};
