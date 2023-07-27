#pragma once

#include "CoreMinimal.h"
#include "Components/LineBatchComponent.h"
#include "DrawDebugHelper.generated.h"

UCLASS()
class DYNASTYPROJECT_API UDrawDebugHelper : public ULineBatchComponent
{
	GENERATED_BODY()
public:
	UDrawDebugHelper();
	UDrawDebugHelper(const FVector& Center, float YawAngle, float ConeHalfAngle, float Near, float Far);

	bool Contains(const FVector& Location) const;

	void DrawDebugWireframe(UWorld* World, FColor Color) const;

private:
	bool CheckToContain(float InnerRadius, float OuterRadius, const FVector& VecToDest, float ComapreAbsDelta, float SourceHeading) const;

private:
	FVector m_Center;
	float m_Yaw = 0;
	float m_HalfAngle = 45.0f;
	float m_Near = 0;
	float m_Far = 10000.0f;
};
