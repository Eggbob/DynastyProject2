
#pragma once

#include "EngineMinimal.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DYAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCheckComboDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCheckHeavyAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnResetStateDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSaveDodgeDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCrashDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAnimMoveDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLaunchDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLightLaunchDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLightCrashDelegate);

UCLASS()
class DYNASTYPROJECT_API UDYAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UDYAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;

	void PlayLightAttackMontage(int atkIdx);
	void PlayLaunchAttackMontage();
	void PlayHeavyAttackMontage();
	void PlayDodgeMontage();
	void PlayDoubleJumpMontage();
	void PlayerAirAttackMontage(int atkIdx);

public:
	FOnCheckComboDelegate OnCheckCombo;
	FOnResetStateDelegate OnResetState;
	FOnSaveDodgeDelegate OnSaveDodge;
	FOnCheckHeavyAttackDelegate OnCheckHeavyAttack;
	FOnAnimMoveDelegate OnAnimMove;
	FOnAttackCheckDelegate OnAttackCheck;
	FOnCrashDelegate OnCrash;
	FOnLaunchDelegate OnLaunch;
	FOnLightLaunchDelegate OnLightLuanch;
	FOnLightCrashDelegate OnLightCrash;

private:
	UFUNCTION()
		void AnimNotify_NextComboCheck();

	UFUNCTION()
		void AnimNotify_CheckHeavyAttack();

	UFUNCTION()
		void AnimNotify_ResetState();

	UFUNCTION()
		void AnimNotify_SaveDodge();

	UFUNCTION()
		void AnimNotify_AnimMove();

	UFUNCTION()
		void AnimNotify_AttackCheck();

	UFUNCTION()
		void AnimNotify_LaunchCharacter();

	UFUNCTION()
		void AnimNotify_LightLaunch();

	UFUNCTION()
		void AnimNotify_LightCrash();

	UFUNCTION()
		void AnimNotify_Crash();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDoubleJump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Air, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> AirComboMontages;

	UPROPERTY()
		APawn* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> LightAttackComboMontage;

	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* LaunchAttackMontage;

	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HeavyAttackMontage;

	UPROPERTY(VisibleAnywhere, Category = Dodge, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DodgeMontage;

	UPROPERTY(VisibleAnywhere, Category = Jump, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DoubleJumpMontage;
};
