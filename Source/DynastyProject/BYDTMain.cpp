#include "BYDTMain.h"

UBYDTMain::UBYDTMain() : Super()
{
	DestructibleImpulse = 800.f;
	DestructibleDamageSpreadScale = 1.f;
	bCausedByWorld = false;
	bScaleMomentumByMass = true;
	DamageFalloff = 1.0f;
	bRadialDamageVelChange = false;
	DamageImpulse = 800.f;
	CurDamageType = E_DmgType::Nothing;
}

void UBYDTMain::SetDamageType(float _DestructibleImpluse, float _DDSpreadScale, bool _CauseByWorld, bool _ScaleMovementbyMass, float _DamgeFalloff, bool _RadialDamageVelChange, float _DamageImpulse, E_DmgType DamageType)
{
	this->DestructibleImpulse = _DestructibleImpluse;
	this->DestructibleDamageSpreadScale = _DDSpreadScale;
	this->bCausedByWorld = _CauseByWorld;
	this->bScaleMomentumByMass = _ScaleMovementbyMass;
	this->DamageFalloff = _DamgeFalloff;
	this->bRadialDamageVelChange = _RadialDamageVelChange;
	this->DamageImpulse = _DamageImpulse;
	CurDamageType = DamageType;
}

void UBYDTMain::SetDamageType(E_DmgType DamageType)
{
	CurDamageType = DamageType;
}

FString UBYDTMain::GetEnumName()
{
	FString TypeName;

	switch (CurDamageType)
	{
	case E_DmgType::Nothing:
		TypeName = TEXT("Nothing");
		break;
	case E_DmgType::DT_Light:
		TypeName = TEXT("Light");
		break;
	case E_DmgType::DT_Launch:
		TypeName = TEXT("DT_Launch");
		break;
	case E_DmgType::DT_KnockBack:
		TypeName = TEXT("DT_KnockBack");
		break;
	case E_DmgType::DT_KnockDown:
		TypeName = TEXT("DT_KnockDown");
		break;
	default:
		break;
	}

	return TypeName;
}

