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
	UE_LOG(LogTemp, Warning, TEXT("Starting Interaction"));

	if (CanInteract())
	{
		UE_LOG(LogTemp, Warning, TEXT("Can Interact"));

		// Setup parameters for the linetrace
		FHitResult OutHit;
		APawn* OwningPawn = Cast<APawn>(GetOwner());
		APlayerCameraManager* PCM = Cast<APlayerController>(OwningPawn->GetController())->PlayerCameraManager;
		const FVector StartLocation = PCM->GetCameraLocation();
		const FVector EndLocation = StartLocation + UKismetMathLibrary::GetForwardVector(PCM->GetCameraRotation()) * TraceDistance;
		FCollisionQueryParams LineTraceParams;
		

		if (bool bHitInteractable = GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility, LineTraceParams))
		{
			if (bDebugInteraction)
				DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor(0, 255, 0), false, 5, 0, 3.f);
			
			UE_LOG(LogTemp, Warning, TEXT("Tracing"));

			if (OutHit.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				IInteractInterface::Execute_OnInteracted(OutHit.GetActor());
				UE_LOG(LogTemp, Warning, TEXT("Actor has interface, calling OnInteracted On hit actor"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor does not implement IInteractInterface"));
			}
		}
		else
		{
			if(bDebugInteraction)
				DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor(255, 0, 0), false, 5, 0, 3.f);
			
			UE_LOG(LogTemp, Warning, TEXT("No actor was hit"));
		}

		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant Interact"));

	}
}

void UInteractionComponent::SetupPlayerInput(UInputComponent* PlayerInput)
{

	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn)
	{
		if (APlayerController* PC = Cast<APlayerController>(OwningPawn->GetController()))
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
			
			if (Subsystem)
			{
				Subsystem->AddMappingContext(InputMapping, 0);
			}
		}
	}

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInput);

	UInteractionInputConfigData* InputConfig = Cast<UInteractionInputConfigData>(InputActions);
	PEI->BindAction(InputConfig->InputInteract, ETriggerEvent::Started, this, &UInteractionComponent::StartInteraction);
}
