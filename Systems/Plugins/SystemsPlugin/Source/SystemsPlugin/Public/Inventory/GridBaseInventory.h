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
	bool bSlotFilled;

	UPROPERTY(BlueprintReadOnly)
	FItemData ItemInCell;

	FGridCell()
	{
		Row = -1;
		Column = -1;
		bSlotFilled = false;
	}
};

UCLASS()
class SYSTEMSPLUGIN_API UGridBaseInventory : public UInventoryBase
{
	GENERATED_BODY()
	
public:
	UGridBaseInventory();


	UFUNCTION(BlueprintPure)
	FGridCell GetGridCellFromIndex(int InIndex);

	UFUNCTION(BlueprintPure)
	TArray<FGridCell> GetGrid();

	void CreateInventory() override;

	UFUNCTION(BlueprintCallable)
	bool AddItem(FItemData ItemToAdd);
	
	bool IsInventoryFull();

	void UpdateInventory();

	UFUNCTION(BlueprintNativeEvent)
	void OnUpdateInventory();

	UFUNCTION(BlueprintImplementableEvent)
	void OnItemAddedToSlot(FItemData InItem, int InIndex);

	/** Returns an array with length 0 if no free slot was found */
	//TArray<int> FindFreeSlot(const FGridCell InCell, EDirection Dir, int Size);

	/** Returns -1 if nothing was found */
	int FindFreeSlot();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int InventoryColumns = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int InventoryRows = 0;

private:
	TArray<FGridCell> Grid;
};
