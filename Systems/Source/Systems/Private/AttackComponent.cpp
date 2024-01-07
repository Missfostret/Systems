// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPawn = Cast<APawn>(GetOwner());
	ensure(OwningPawn);

	SetupPlayerInput(OwningPawn->InputComponent);
	
}

void UAttackComponent::SetupPlayerInput(UInputComponent* PlayerInput)
{
	auto PC = Cast<APlayerController>(OwningPawn->GetController());
	if (OwningPawn == nullptr && PC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning pawn and player controller not valid"));
		return;
	}

	if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		UE_LOG(LogTemp, Warning, TEXT("SubSystem is valid, adding mapping context in Attack Component"));
		Subsystem->AddMappingContext(InputMapping, 0);
	}

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInput);
	PEI->BindAction(InputAction, ETriggerEvent::Started, this, &UAttackComponent::TryAttack);
	//PEI->BindAction(InputConfig->InputInteract, ETriggerEvent::Completed, this, &UInteractionComponent::StopInteraction);

}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackComponent::TryAttack()
{
	if (bIsAttacking)
	{
		UE_LOG(LogTemp, Error, TEXT("Returning, Is Already Attacking!"));
		return;
	}

	PerformAttack();
	bIsAttacking = true;
	return;
}

void UAttackComponent::PerformAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Performing Attack"));

	bIsAttacking = false;
}

