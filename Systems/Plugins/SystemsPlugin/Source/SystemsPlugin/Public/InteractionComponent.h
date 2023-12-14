// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UDataAsset;
class UInteractInterface;
class APawn;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UDataAsset* InputActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float TraceDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	bool bDebugInteraction = true;

protected:
	bool CanInteract();

	void StartInteraction();

	FHitResult LineTraceInteraction();

	bool TryInteract(bool bHasInterface, AActor* HitActor, APawn* InOwningPawn);

	bool InteractionInstant(float InInteractionTime);

	void SetupPlayerInput(class UInputComponent* PlayerInput);

private:
	void DebugInteractionLineTrace(FVector StartLocation, FVector EndLocation, bool bHit);

private:
	APawn* OwningPawn;
};
