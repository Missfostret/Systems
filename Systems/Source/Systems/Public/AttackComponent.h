// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"


class UInputAction;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYSTEMS_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetupPlayerInput(UInputComponent* PlayerInput);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TryAttack();

	void PerformAttack();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction;

	bool bIsAttacking = false;

private:
	APawn* OwningPawn;
	// TODO@: Make an attack, make sure you can attack
	// Left click to perform attack
	// Linetrace? Spheretrace? probably spheretrace forward
	// MultiSphereTrace to find the everything inside of the attack area or just the closest  thing hit so you cant multifarm..? probably hit everything inside
};
