// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BYDTMain.h"
#include "DYEnemyAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnEnemyFallDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEnemyRecoverDelegate);

UCLASS()
class DYNASTYPROJECT_API UDYEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	FOnEnemyFallDelegate OnEnemyFall;
	FOnEnemyRecoverDelegate OnEnemyRecover;

	UFUNCTION()
		void AnimNotify_EnemyFall() { OnEnemyFall.Broadcast(); };

	UFUNCTION()
		void AnimNotify_RecoverEnemy() { OnEnemyRecover.Broadcast(); };

	UFUNCTION(BlueprintImplementableEvent, Category = Hit)
		void PlayHitAnim(UAnimMontage* Montage);

	void CheckHitAnimation(E_DmgType DamageType);

private:
	UDYEnemyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;
	void EnemyFallingRoutine();



private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
		E_DmgType CurDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> HitAnims;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		UAnimMontage* RecoverAnim;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
		bool bIsHit;
};
