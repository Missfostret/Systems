#pragma once

#include "Direction.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FString Description;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDirection> Direction;

	UPROPERTY(BlueprintReadOnly)
	int ItemSpace;

	FItemData()
	{
		Name = "None";
		Description = "None";
		Direction = EDirection::None;
		ItemSpace = 0;
	}
};
