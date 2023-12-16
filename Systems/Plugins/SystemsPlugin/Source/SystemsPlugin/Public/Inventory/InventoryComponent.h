// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYSTEMSPLUGIN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	TSubclassOf<UInventoryBase> InventoryType;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UInventoryBase* Inventory;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
