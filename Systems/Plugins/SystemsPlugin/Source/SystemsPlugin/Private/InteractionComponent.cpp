#include "InteractionComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "InteractInterface.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "InteractionInputConfigData.h"
#include "InteractionInputConfigData.h"


UInteractionComponent::UInteractionComponent()
{
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerInput(Cast<APawn>(GetOwner())->InputComponent);

	OwningPawn = Cast<APawn>(GetOwner());
	ensure(OwningPawn);
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TODO@: Add the OngoingInteraction() so the blueprint OnInteractionTick() is being updated
	// Only update if an interactiontime is over 0.0f otherwise just make the interaction instant
	if (InteractionTime > 0.0f)
	{
		OnGoingInteraction(CurrentInteractionTime);
		CurrentInteractionTime += DeltaTime;
		if (CurrentInteractionTime >= InteractionTime)
		{
			Interact();
		}
	}
}

bool UInteractionComponent::CanInteract()
{
	return !bIsInteracting;
}

void UInteractionComponent::StartInteraction()
{
	// TODO@ Add interaction time, some interactions should be instant and others require a set amount of time

	if (CanInteract())
	{
		UE_LOG(LogTemp, Warning, TEXT("Already Interacting Or Cant Interact"));
		return;
	}

	bIsInteracting = true;

	auto OutHit = LineTraceInteraction();
		
	if (!OutHit.bBlockingHit)
	{
		DebugInteractionLineTrace(OutHit.TraceStart, OutHit.TraceEnd, false);
		UE_LOG(LogTemp, Warning, TEXT("No actor was hit"));
		return;
	}

	DebugInteractionLineTrace(OutHit.TraceStart, OutHit.TraceEnd, true);

	bool bHasInteractionInterface = OutHit.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());
	TryInteract(bHasInteractionInterface, OutHit.GetActor(), OwningPawn);
}

FHitResult UInteractionComponent::LineTraceInteraction()
{
	// Setup parameters for the linetrace
	FHitResult OutHit;
	APlayerCameraManager* PCM = Cast<APlayerController>(OwningPawn->GetController())->PlayerCameraManager;
	const FVector StartLocation = PCM->GetCameraLocation();
	const FVector EndLocation = StartLocation + UKismetMathLibrary::GetForwardVector(PCM->GetCameraRotation()) * TraceDistance;
	FCollisionQueryParams LineTraceParams;

	GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility, LineTraceParams);

	return OutHit;
}

void UInteractionComponent::SetupPlayerInput(UInputComponent* PlayerInput)
{
	if (OwningPawn)
	{
		if (auto PC = Cast<APlayerController>(OwningPawn->GetController()))
		{
			auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
			
			if (Subsystem)
			{
				Subsystem->AddMappingContext(InputMapping, 0);
			}
		}
	}

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInput);

	UInteractionInputConfigData* InputConfig = Cast<UInteractionInputConfigData>(InputActions);
	PEI->BindAction(InputConfig->InputInteract, ETriggerEvent::Ongoing, this, &UInteractionComponent::StartInteraction);
	PEI->BindAction(InputConfig->InputInteract, ETriggerEvent::Canceled, this, &UInteractionComponent::CancelInteraction);
}

void UInteractionComponent::OnGoingInteraction(float InInteractionTime)
{
	// TODO@: This is where the component updates what happens on tick
	OnInteractionTick(InteractionTime);
}

void UInteractionComponent::OnInteractionTick_Implementation(float InInteractionTime)
{

}

void UInteractionComponent::CancelInteraction()
{
	// TODO@: Implement Cancel Interaction logic
	StopInteraction();
	UE_LOG(LogTemp, Warning, TEXT("Interaction Got Cancelled"));
}

void UInteractionComponent::StopInteraction()
{
	// TODO@: Implement Stop Interaction logic
	ResetInteraction();
	UE_LOG(LogTemp, Warning, TEXT("Interaction Got Stopped"));
}

void UInteractionComponent::ResetInteraction()
{
	bIsInteracting = false;
	bInstantInteraction = false;
	InteractionTime = -1.0f;
	CurrentInteractionTime = 0.0f;
	InteractingActor = nullptr;
}

void UInteractionComponent::DebugInteractionLineTrace(FVector StartLocation, FVector EndLocation, bool bHit)
{
	if (bDebugInteraction)
	{
		FColor DebugColor = (bHit) ? FColor(0,255,0) : FColor(255, 0, 0);
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, DebugColor, false, 5, 0, 3.f);
	}
}

bool UInteractionComponent::TryInteract(bool bHasInterface, AActor* HitActor, APawn* InOwningPawn)
{
	if (!bHasInterface)
	{
		CancelInteraction();
		return false;
	}
	
	InteractionTime = IInteractInterface::Execute_InteractionTime(HitActor);
	InteractingActor = HitActor;

	if (!IsInteractionInstant(InteractionTime))
	{
		bInstantInteraction = false;
		//TODO@ Add interaction time here
		// Maybe enable component tick and tick an interaction time up to the set interaction time
		return false;
	}

	//TODO@ Interaction is instant, should only be able to trace once if hit something
	// Set CanInteract to false
	bInstantInteraction = true;
	IInteractInterface::Execute_OnInteracted(InteractingActor, OwningPawn);
	UE_LOG(LogTemp, Warning, TEXT("Actor has interface, calling OnInteracted On hit actor"));
	return true;
}

void UInteractionComponent::Interact()
{
	IInteractInterface::Execute_OnInteracted(InteractingActor, OwningPawn);
	ResetInteraction();
}

bool UInteractionComponent::IsInteractionInstant(float InInteractionTime)
{
	return bInstantInteraction = InInteractionTime <= 0.0f;
}