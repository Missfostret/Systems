#include "Interaction/InteractionComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Interaction/InteractInterface.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Interaction/InteractionInputConfigData.h"

#include "Widgets/SystemsUserWidget.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPawn = Cast<APawn>(GetOwner());
	ensure(OwningPawn);

	SetupPlayerInput(OwningPawn->InputComponent);

	InteractionWidget = Cast<USystemsUserWidget>(CreateWidget(GetWorld(), InteractionWidgetClass));
	ensure(InteractionWidget);

}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (InteractionTime > 0.0f)
	{
		OnGoingInteraction(CurrentInteractionTime);
		CurrentInteractionTime += DeltaTime;
		if (CurrentInteractionTime >= InteractionTime)
		{
			Interact();
			SetComponentTickEnabled(false);
		}
	}
}

bool UInteractionComponent::CanInteract()
{
	return !bIsInteracting;
}

void UInteractionComponent::StartInteraction()
{

	if (!CanInteract())
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
	UE_LOG(LogTemp, Warning, TEXT("INTERACTING"));

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

	auto PC = Cast<APlayerController>(OwningPawn->GetController());
	if (OwningPawn == nullptr && PC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning pawn and player controller not valid"));
		return;
	}

	if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		UE_LOG(LogTemp, Warning, TEXT("SubSystem is valid, adding mapping context"));
		Subsystem->AddMappingContext(InputMapping, 0);
	}

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInput);

	UInteractionInputConfigData* InputConfig = Cast<UInteractionInputConfigData>(InputActions);
	PEI->BindAction(InputConfig->InputInteract, ETriggerEvent::Started, this, &UInteractionComponent::StartInteraction);
	PEI->BindAction(InputConfig->InputInteract, ETriggerEvent::Completed, this, &UInteractionComponent::StopInteraction);
}

void UInteractionComponent::OnGoingInteraction(float InInteractionTime)
{
	if (ActorInteractedWith)
	{
		float NormalizedValue = InInteractionTime / InteractionTime;
		OnInteractionTick(InInteractionTime, NormalizedValue);
		IInteractInterface::Execute_OngoingInteract(ActorInteractedWith, CurrentInteractionTime, NormalizedValue);
		IInteractInterface::Execute_OngoingInteract(InteractionWidget, CurrentInteractionTime, NormalizedValue);
	}
}

void UInteractionComponent::OnInteractionTick_Implementation(float InInteractionTime, float NormalizedInteractionTime)
{

}

void UInteractionComponent::CancelInteraction()
{
	StopInteraction();
	UE_LOG(LogTemp, Warning, TEXT("Interaction Got Cancelled"));
}

void UInteractionComponent::StopInteraction()
{
	ResetInteraction();
	UE_LOG(LogTemp, Warning, TEXT("Interaction Got Stopped"));
}

void UInteractionComponent::ResetInteraction()
{
	bIsInteracting = false;
	bInstantInteraction = false;
	InteractionTime = -1.0f;
	CurrentInteractionTime = 0.0f;
	ActorInteractedWith = nullptr;

	if(InteractionWidget->IsInViewport())
		InteractionWidget->RemoveFromParent();
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
	ActorInteractedWith = HitActor;

	if (!IsInteractionInstant(InteractionTime))
	{
		bInstantInteraction = false;
		SetComponentTickEnabled(true);
		if (!InteractionWidget->IsInViewport())
			InteractionWidget->AddToViewport(0);
		return false;
	}

	bInstantInteraction = true;
	Interact();
	return true;
}

void UInteractionComponent::Interact()
{
	IInteractInterface::Execute_OnInteracted(ActorInteractedWith, OwningPawn);
	ResetInteraction();
	UE_LOG(LogTemp, Warning, TEXT("Interaction finished"));
}

bool UInteractionComponent::IsInteractionInstant(float InInteractionTime)
{
	return bInstantInteraction = InInteractionTime <= 0.0f;
}