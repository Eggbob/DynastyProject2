// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "BYDTMain.generated.h"


UENUM(BlueprintType)
enum class E_DmgType : uint8
{
	Nothing UMETA(DisplayName = "Nothing"),
	DT_Light UMETA(DisplayName = "LightAttackDamage"),
	DT_Launch UMETA(DisplayName = "LaunchDamage"),
	DT_AirATK UMETA(DisplayName = "AirAttackDamage"),
	DT_KnockBack UMETA(DisplayName = "KnockBackDamage"),
	DT_KnockDown UMETA(DisplayName = "KnockDownDamage"),
};



UCLASS()
class DYNASTYPROJECT_API UBYDTMain : public UDamageType
{
	GENERATED_BODY()
	
public:
	UBYDTMain();

	void SetDamageType(float DestructibleImpluse, float DDSpreadScale, bool CauseByWorld, bool ScaleMovementbyMass, 
		float DamgeFalloff, bool RadialDamageVelChange, float DamageImpulse, E_DmgType DamageType);

	void SetDamageType(E_DmgType DamageType);

	E_DmgType GetDamageType() { return CurDamageType; }
	
	FString GetEnumName();

private:
	E_DmgType CurDamageType = E_DmgType::Nothing;

public:
	float MoveValue;
};
