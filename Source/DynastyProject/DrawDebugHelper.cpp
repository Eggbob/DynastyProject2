// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawDebugHelper.h"
UDrawDebugHelper::UDrawDebugHelper() {}

UDrawDebugHelper::UDrawDebugHelper(const FVector& Center, float YawAngle, float ConeHalfAngle, float Near, float Far)
	: m_Center(Center), m_Yaw(YawAngle), m_HalfAngle(ConeHalfAngle), m_Near(Near), m_Far(Far)
{
}

bool UDrawDebugHelper::Contains(const FVector& Location) const
{
	return CheckToContain(m_Near, m_Far, Location - m_Center, m_HalfAngle, m_Yaw);
}

bool UDrawDebugHelper::CheckToContain(float InnerRadius, float OuterRadius, const FVector& VecToDest, float ComapreAbsDelta, float SourceHeading) const
{
	float PolarRadius = VecToDest.SizeSquared2D();
	if (InnerRadius > 0 && PolarRadius < FMath::Square(InnerRadius))
	{
		return false;
	}
	if (PolarRadius > FMath::Square(OuterRadius))
	{
		return false;
	}

	float HeadingToDest = FMath::RadiansToDegrees(VecToDest.HeadingAngle());
	float DeltaAngle = FMath::FindDeltaAngleDegrees(SourceHeading, HeadingToDest);
	if (FMath::Abs(DeltaAngle) > ComapreAbsDelta)
	{
		return false;
	}
	return true;
}

void UDrawDebugHelper::DrawDebugWireframe(UWorld* World, FColor LineColor) const
{

	UE_LOG(LogTemp, Warning, TEXT("DRWADFSMFLSDF"));
	TArray<float> Angles;
	int32 SegmentPerDegree = 10;
	int32 NumOfSegments = FMath::Max(1, static_cast<int32>(m_HalfAngle) * 2 / SegmentPerDegree);
	float PortionAngle = m_HalfAngle * 2 / static_cast<float>(NumOfSegments);
	for (int I = 0; I < NumOfSegments + 1; ++I)
	{
		Angles.Add(-m_HalfAngle + PortionAngle * I);
	}

	TArray<FVector> Vertices;
	// add an inner arc
	for (int I = 0; I < Angles.Num(); ++I)
	{
		FVector Point = FRotator(0, Angles[I] + m_Yaw, 0).RotateVector(FVector(1, 0, 0)) * m_Near;
		Vertices.Add(Point);
	}
	// add an outer arc
	for (int I = Angles.Num() - 1; I >= 0; --I)
	{
		FVector Point = FRotator(0, Angles[I] + m_Yaw, 0).RotateVector(FVector(1, 0, 0)) * m_Far;
		Vertices.Add(Point);
	}
	Vertices.Emplace(Vertices[0]); // for a radii

	// Draw it
	for (int I = 1; I < Vertices.Num(); ++I)
	{
		DrawDebugLine(World, m_Center + Vertices[I - 1], m_Center + Vertices[I], LineColor, false, 0.0f);
	}
}