// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryBase.h"
#include "General/ItemData.h"
#include "General/Direction.h"
#include "GridBaseInventory.generated.h"

/**
 * 
 */


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
class SYSTEMSPLUGIN_API UGridBaseInventory : public UInventoryBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	FGridCell GetGridCellFromIndex(int InIndex);

	UFUNCTION(BlueprintPure)
	TArray<FGridCell> GetGrid();

	void CreateInventory() override;

	void AddItem(FItemData ItemToAdd);
	
	bool IsInventoryFull();

	TArray<int> FindFreeSlot(const FGridCell InCell, EDirection Dir, int Size);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int InventoryColumns = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int InventoryRows = 0;

private:
	TArray<FGridCell> Grid;
};
