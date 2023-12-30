#pragma once

#include "Direction.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EDirection> Direction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemSpace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanStack;

	FItemData()
	{
		Name = "None";
		Description = "None";
		Direction = EDirection::None;
		ItemSpace = 0;
		bCanStack = false;
	}
};
