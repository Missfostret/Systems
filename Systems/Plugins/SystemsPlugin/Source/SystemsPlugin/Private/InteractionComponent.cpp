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
}

bool UInteractionComponent::CanInteract()
{
	return true;
}

void UInteractionComponent::StartInteraction()
{
	// TODO@ Add interaction time, some interactions should be instant and others require a set amount of time

	if (CanInteract())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant Interact"));
		return;
	}

	auto OutHit = LineTraceInteraction();
		
	if (OutHit.bBlockingHit)
	{
		DebugInteractionLineTrace(OutHit.TraceStart, OutHit.TraceEnd, OutHit.bBlockingHit);

		bool bHasCorrectInterface = OutHit.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());
		TryInteract(bHasCorrectInterface, OutHit.GetActor(), OwningPawn);
	}
	else
	{
		DebugInteractionLineTrace(OutHit.TraceStart, OutHit.TraceEnd, OutHit.bBlockingHit);
		UE_LOG(LogTemp, Warning, TEXT("No actor was hit"));
	}
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
		return false;
	}
	
	auto InteractionTime = IInteractInterface::Execute_InteractionTime(HitActor);

	if (!InteractionInstant(InteractionTime))
	{
		//TODO@ Add interaction time here
		// Maybe enable component tick and tick an interaction time up to the set interaction time
		return false;
	}

	//TODO@ Interaction is instant, should only be able to trace once if hit something
	// Set CanInteract to false
	IInteractInterface::Execute_OnInteracted(HitActor, OwningPawn);
	UE_LOG(LogTemp, Warning, TEXT("Actor has interface, calling OnInteracted On hit actor"));
	return true;
}

bool UInteractionComponent::InteractionInstant(float InInteractionTime)
{
	return InInteractionTime <= 0.0f;
}