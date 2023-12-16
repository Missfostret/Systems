// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UDataAsset;
class UInteractInterface;
class APawn;
class USystemsUserWidget;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYSTEMSPLUGIN_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteractionTick(float InInteractionTime, float NormalizedInteractionTime);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UDataAsset* InputActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float TraceDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	bool bDebugInteraction = true;

	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	USystemsUserWidget* InteractionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<USystemsUserWidget> InteractionWidgetClass;

protected:
	// Interaction
	FHitResult LineTraceInteraction();
	bool CanInteract();
	void StartInteraction();
	bool TryInteract(bool bHasInterface, AActor* HitActor, APawn* InOwningPawn);
	void Interact();
	bool IsInteractionInstant(float InInteractionTime);
	void OnGoingInteraction(float InInteractionTime);
	void CancelInteraction();
	void StopInteraction();
	void ResetInteraction();

	// Input
	void SetupPlayerInput(class UInputComponent* PlayerInput);

private:
	// Debug
	void DebugInteractionLineTrace(FVector StartLocation, FVector EndLocation, bool bHit);

private:
	APawn* OwningPawn;

	class AActor* ActorInteractedWith;
	bool bIsInteracting = false;
	bool bInstantInteraction = false;
	float InteractionTime = -1.0f;
	float CurrentInteractionTime = 0.0f;
};
