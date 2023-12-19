#pragma once

UENUM(BlueprintType)
enum EDirection : uint8
{
	None = 0 UMETA(DisplayName = "NONE"),
	Left = 1 UMETA(DisplayName = "LEFT"),
	Down = 2 UMETA(DisplayName = "DOWN"),
	Right = 3 UMETA(DisplayName = "RIGHT"),
	Up = 4 UMETA(DisplayName = "UP")
};


