// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryBase.h"

void UInventoryBase::CreateInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Inventory of type %s"), *this->GetName());

	for (int i = 0; i < InventoryRows; i++)
	{
		for (int y = 0; y < InventoryColumns; y++)
		{
			FGridCell NewCell;
			NewCell.Row = i;
			NewCell.Column = y;
			FItemData NewItem;
			NewCell.ItemInCell = NewItem;
			Grid.Add(NewCell);
			UE_LOG(LogTemp, Warning, TEXT("Creating Item %s in Grid cell Row %i . %d"), *NewCell.ItemInCell.Name, NewCell.Row, NewCell.Column);
		}
	}
}

FGridCell UInventoryBase::GetGridCellFromIndex(int InIndex)
{
	return Grid[InIndex];
}
