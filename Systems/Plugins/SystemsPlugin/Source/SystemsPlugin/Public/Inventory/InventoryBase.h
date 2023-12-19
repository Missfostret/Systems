// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryBase.generated.h"

/**
 * 
 */



UCLASS(Blueprintable)
class SYSTEMSPLUGIN_API UInventoryBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void CreateInventory();

};


// TODO@: Inventories
/**
Unlimited Inventory aka "Weight based inventory", maybe have weight of items to hinder the player to carry too much but have no slots instead
"Utility Belt" inventory, only carry a selected number of items and switch between items with hotkeys like 1-9 or something, usually games like CS and valorant, half life etc.
Menu based inventory, bring up a menu where you can see and manage all your items, often slot based.

Inventory pick up systems:
Press a button to pick up a single item, can be paired with open inventory and display all items in the vicinity
*/