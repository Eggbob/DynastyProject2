

#include "DYAnimInstance.h"

UDYAnimInstance::UDYAnimInstance()
{
	CurrentSpeed = 0.f;
	IsFalling = false;

	for (int i = 0; i < 4; i++)
	{
		FString path = FString::Printf(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Animations/Montage/Combat/SK_Attack_Montage%d.SK_Attack_Montage%d'"), i+1, i+1);
		ConstructorHelpers::FObjectFinder<UAnimMontage> LIGHT_ATK_MONTAGES(*path);

		if (LIGHT_ATK_MONTAGES.Succeeded())
		{
			LightAttackComboMontage.Add(LIGHT_ATK_MONTAGES.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *path);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		FString path = FString::Printf(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Animations/Montage/Combat/AN_AirAttack%d.AN_AirAttack%d'"), i + 1, i + 1);
		ConstructorHelpers::FObjectFinder<UAnimMontage> AIR_ATK_MONTAGES(*path);

		if (AIR_ATK_MONTAGES.Succeeded())
		{
			AirComboMontages.Add(AIR_ATK_MONTAGES.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *path);
		}
	}


	ConstructorHelpers::FObjectFinder<UAnimMontage> HEAVYATK_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Animations/Montage/Combat/AN_HeavyAttack.AN_HeavyAttack'"));

	if (HEAVYATK_MONTAGE.Succeeded())
	{
		HeavyAttackMontage = HEAVYATK_MONTAGE.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Animations/Montage/AN_Dodge.AN_Dodge'"));
	
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> DOUBLEJUMP_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Animations/Montage/AN_DoubleJump.AN_DoubleJump'"));

	if (DOUBLEJUMP_MONTAGE.Succeeded())
	{
		DoubleJumpMontage = DOUBLEJUMP_MONTAGE.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> LAUNCH_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Animations/Montage/Combat/AN_LaunchUp.AN_LaunchUp'"));

	if (LAUNCH_MONTAGE.Succeeded())
	{
		LaunchAttackMontage = LAUNCH_MONTAGE.Object;
	}


}


void UDYAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (::IsValid(Owner))
	{
		CurrentSpeed = Owner->GetVelocity().Size();
		IsFalling = Owner->GetMovementComponent()->IsFalling() || Owner->GetMovementComponent()->IsFlying();
	}
}

void UDYAnimInstance::NativeInitializeAnimation()
{
	Owner = TryGetPawnOwner();
}

#pragma region PlayMontage

void UDYAnimInstance::PlayLightAttackMontage(int atkIdx)
{
	Montage_Play(LightAttackComboMontage[atkIdx], 1.f);
}


void UDYAnimInstance::PlayLaunchAttackMontage()
{
	Montage_Play(LaunchAttackMontage, 1.f);
}

void UDYAnimInstance::PlayHeavyAttackMontage()
{
	Montage_Play(HeavyAttackMontage, 1.f);
}

/// <summary>
/// Dodge 애니메이션 실행
/// </summary>
void UDYAnimInstance::PlayDodgeMontage()
{
	Montage_Play(DodgeMontage, 1.f);
}

void UDYAnimInstance::PlayDoubleJumpMontage()
{
	Montage_Play(DoubleJumpMontage, 1.f);
}

void UDYAnimInstance::PlayerAirAttackMontage(int atkIdx)
{
	Montage_Play(AirComboMontages[atkIdx], 1.f);
}


#pragma endregion




#pragma region AnimNotifys

void UDYAnimInstance::AnimNotify_NextComboCheck()
{
	OnCheckCombo.Broadcast();
}

void UDYAnimInstance::AnimNotify_CheckHeavyAttack()
{
	OnCheckHeavyAttack.Broadcast();
}

void UDYAnimInstance::AnimNotify_ResetState()
{
	OnResetState.Broadcast();
}

void UDYAnimInstance::AnimNotify_SaveDodge()
{
	OnSaveDodge.Broadcast();
}

void UDYAnimInstance::AnimNotify_AnimMove()
{
	OnAnimMove.Broadcast();
}

void UDYAnimInstance::AnimNotify_AttackCheck()
{
	OnAttackCheck.Broadcast();
}

void UDYAnimInstance::AnimNotify_LaunchCharacter()
{
	OnLaunch.Broadcast();
}

void UDYAnimInstance::AnimNotify_LightLaunch()
{
	OnLightLuanch.Broadcast();
}

void UDYAnimInstance::AnimNotify_LightCrash()
{
	OnLightCrash.Broadcast();
}

void UDYAnimInstance::AnimNotify_Crash()
{
	OnCrash.Broadcast();
}

#pragma endregion




