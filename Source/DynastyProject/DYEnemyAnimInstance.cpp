
#include "DYEnemyAnimInstance.h"

UDYEnemyAnimInstance::UDYEnemyAnimInstance()
{
}

void UDYEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
}

void UDYEnemyAnimInstance::NativeInitializeAnimation()
{
	OnEnemyFall.AddUObject(this, &UDYEnemyAnimInstance::EnemyFallingRoutine);
}

/// <summary>
/// 피격 애니메이션 체크
/// </summary>
/// <param name="DamageType"></param>
void UDYEnemyAnimInstance::CheckHitAnimation(E_DmgType DamageType)
{
	bIsHit = true;
	CurDamageType = DamageType;

	UAnimMontage* Montage = HitAnims[(uint8)DamageType];
	
	if (HitAnims[(uint8)DamageType] != nullptr)
	{
		PlayHitAnim(Montage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit Animation"));
	}
}



/// <summary>
/// 공중에서 낙하중인 적 애니메이션 처리
/// </summary>
void UDYEnemyAnimInstance::EnemyFallingRoutine()
{
	UAnimMontage* Montage = HitAnims[(uint8)E_DmgType::DT_KnockDown];

	if (Montage != nullptr)
	{
		PlayHitAnim(Montage);
	}

	//회복 애니메이션 실행
	FTimerHandle Timer;
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([this] {  Montage_Play(RecoverAnim); });
	GetWorld()->GetTimerManager().SetTimer(Timer, TimerCallback, 0.6f, false);
}





