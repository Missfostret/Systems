// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryBase.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FString Description;

	FItemData() 
	{
		Name = "None";
		Description = "None";
	}
};

USTRUCT(BlueprintType)
struct FGridCell
{

	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int Row;

	UPROPERTY(BlueprintReadOnly)
	int Column;

	UPROPERTY(BlueprintReadOnly)
	FItemData ItemInCell;


	FGridCell() 
	{
		Row = -1;
		Column = -1;
	}
};


UCLASS()
class SYSTEMSPLUGIN_API UInventoryBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void CreateInventory();

	FGridCell GetGridCellFromIndex(int InIndex);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int InventoryColumns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int InventoryRows;

private:
	TArray<FGridCell> Grid;
};
