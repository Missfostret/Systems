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
	if (IsInventoryFull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Full, Canceling Add Item"));
		return;
	}

	//if (ItemToAdd.bCanStack)
	//{
	//	// Find stack and add, if no stack was found, then add to new slot
	//	// return
	//}

	// if you get here, the item does not stack and you can just find empty slots for that item and add it to the inventory
	int FreeSlotIndex;
	FreeSlotIndex = FindFreeSlot();
	if(FreeSlotIndex == -1)
	{
		// Found no free slot
		UE_LOG(LogTemp, Warning, TEXT("Found no free slot"));

		return;
	}

	// Fill free slot with item and tell the slot that its full
	Grid[FreeSlotIndex].ItemInCell = ItemToAdd;
	Grid[FreeSlotIndex].bSlotFilled = true;
	UE_LOG(LogTemp, Warning, TEXT("Filling slot %s x %d"), Grid[FreeSlotIndex].Column, Grid[FreeSlotIndex].Row);

	UpdateInventory();

	// TODO@: 2. Check if theres a slot free if the item takes 1 inventory space, if yes, 
	// check if item can stack stack and if we already have a stack thats not full then add that item to stack.
	// if the item quantity of item your picking up is more than one then try and add as many items to the stack and then re-add the rest of the items. and repeat the logic.
	// if the item takes up more than 1 slot then check the direction of that item and what slot is next to it in that direction, is the slot free? then add item to those slots otherwise try and find another slot.
	// if the inventory is full do nothing, the way we check if the inventory is full should be a loop through the items and see if the slots are filled.
}

bool UGridBaseInventory::IsInventoryFull()
{
	for(FGridCell Cell : Grid)
	{
		if (!Cell.bSlotFilled)
		{
			return false;
		}
	}
	return true;
}

void UGridBaseInventory::UpdateInventory()
{
	// Call blueprint event so more control such as updating inventory widgets etc can happen
	OnUpdateInventory_Implementation();
}


// Leave this empty since its a blueprint event
void UGridBaseInventory::OnUpdateInventory_Implementation()
{

}

//TArray<int> UGridBaseInventory::FindFreeSlot(const FGridCell InCell,EDirection Dir, int Size)
//{
//	//TODO@: 1.
//	//if right check current index + size ofc check if any of the cells are on a new row then we cant add item there since Item wouldnt fit in a straight line
//	//if left check current index - size same as with right
//	//if down do a for loop and check each cell with the inventoryColumn size and if one of the cells there is not empty you cant add item there, check this for each cell in the inventory grid
//	//if up the same as with left but instead do it in reverse, start at the item size in the for loop since the item would fit pointed upwards if its size is 3 and you start at row 0 or 1
//
//	/*TArray<int> FreeCellIndexes;
//	int Index = -1;
//	for (int i = 0; i < Grid.Num(); i++)
//	{
//		if (!Grid[i].bSlotFilled)
//		{
//			Index = i;
//			break;
//		}
//	}*/
//
//	/*switch (Dir)
//	{
//		case EDirection::None:
//			break;
//		case EDirection::Up:
//			break;
//		case EDirection::Left:
//			for (int i = Index; i > Size - Index; i--)
//			{
//				const FString newName = Grid[i].ItemInCell.Name;
//				if (Grid[i].ItemInCell.Name == "None")
//				{
//					FreeCellIndexes.Empty();
//					return FreeCellIndexes;
//				}
//				FreeCellIndexes.Add(i);
//			}
//			return FreeCellIndexes;
//			break;
//		case EDirection::Down:
//			break;
//		case EDirection::Right:
//			for (int i = Index; i < Size + Index; i++)
//			{
//				const FString newName = Grid[i].ItemInCell.Name;
//				if (Grid[i].ItemInCell.Name == "None")
//				{
//					FreeCellIndexes.Empty();
//					return FreeCellIndexes;
//				}
//				FreeCellIndexes.Add(i);
//			}
//			return FreeCellIndexes;
//			break;
//	}
//	return FreeCellIndexes;*/
//
//
//	//for(FGridCell Cell : Grid)
//	//{
//	//	if (!Cell.bSlotFilled)
//	//	{
//	//		return Cell;
//	//	}
//	//}
//	
//}

int UGridBaseInventory::FindFreeSlot()
{
	for(int i = 0; i < Grid.Num(); i++)
	{
		if (!Grid[i].bSlotFilled)
		{
			return i;
		}
	}
	return -1;
}
