// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/RPGInventory.h"

void URPGInventory::CreateInventory()
{
	Super::CreateInventory();

	UE_LOG(LogTemp, Warning, TEXT("Creating Inventory of type %s"), *GetName());
}
