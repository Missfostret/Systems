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
