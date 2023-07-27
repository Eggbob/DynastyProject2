
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class E_MouseEvent : uint8
{
	Press UMETA(DisplayName = "Press"),
    PointerDown UMETA(DisplayName = "PointerDown"),
	PointerUp UMETA(DisplayName = "PointerUp"),
	Click UMETA(DisplayName = "Click"),
};

class DYNASTYPROJECT_API BYEnum
{
public:
	BYEnum();
	~BYEnum();
};
