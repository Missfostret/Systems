// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GridBaseInventory.h"
#include "General/ItemData.h"

FGridCell UGridBaseInventory::GetGridCellFromIndex(int InIndex)
{
	return Grid[InIndex];
}

TArray<FGridCell> UGridBaseInventory::GetGrid()
{
	return Grid;
}

void UGridBaseInventory::CreateInventory()
{
	Super::CreateInventory();

	for (int i = 0; i < InventoryRows; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("New Row"));

		for (int y = 0; y < InventoryColumns; y++)
		{
			FGridCell NewCell;
			NewCell.Row = i;
			NewCell.Column = y;
			FItemData NewItem;
			NewCell.ItemInCell = NewItem;
			Grid.Add(NewCell);
			UE_LOG(LogTemp, Warning, TEXT("Creating Item %s in Grid cell Row %i . %d"), *NewCell.ItemInCell.Name, NewCell.Row, NewCell.Column);
			UE_LOG(LogTemp, Warning, TEXT("New Column"));
		}
	}
}

void UGridBaseInventory::AddItem(FItemData ItemToAdd)
{
	int InventorySize = InventoryColumns * InventoryRows;
	if (Grid.Num() >= InventorySize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Full, Canceling Add Item"));
		return;
	}

	// TODO@: Check if theres a slot free if the item takes 1 inventory space, if yes, check if item can stack stack and if we already have a stack thats not full then add that item to stack.
	// if the item quantity of item your picking up is more than one then try and add as many items to the stack and then readd the rest of the items. and repeat the logic.
	// if the item takes up more than 1 slot then check the direction of that item and what slot is next to it in that direction, is the slot free? then add item to those slots otherwise try and find another slot.
	// if the inventory is full do nothing, the way we check if the inventory is full should be a loop through the items and see if the slots are filled.

}
