// Project PlayerCharacter

#pragma once

#include "EngineMinimal.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BYEnum.h"
#include "BYDTMain.h"
#include "DYCharacter.generated.h"

UENUM(BlueprintType)
enum class E_PlayerState : uint8
{
	Nothing UMETA(DisplayName = "Nothing"),
	LightAttack UMETA(DisplayName = "Attack"),
	HeavyAttack UMETA(DisplayName = "HeavyAttack"),
	LaunchAttack UMETA(DisplayName = "Launch"),
	Dodge UMETA(DisplayName = "Dodge"),
};


UCLASS()
class DYNASTYPROJECT_API ADYCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADYCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void PreInitializeComponents() override;



	void BindCharacterAxis(UInputComponent* PlayerInputComponent);

	E_PlayerState GetCurrentPlayerState() const;

	UFUNCTION(BlueprintImplementableEvent, Category = Buffer)
		void Buffer(float BufferAmount);

	UFUNCTION(BlueprintImplementableEvent, Category = Buffer)
		void StopBuffer();
	
	UFUNCTION(BlueprintImplementableEvent, Category = Dodge)
		void DodgeMoving();

	UFUNCTION(BlueprintImplementableEvent, Category = Combat)
		void MoveDuringAnim();

	UFUNCTION(BlueprintCallable, Category = Combat)
		float GetAnimMoveSpeed();

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

private:
	//MoveMent
	void UpDown(float AxisValue);
	void LeftRight(float AxisValue);
	void LookUp(float AxisValue);
	void Turn(float AxisValue);

	//LightAttack
	void LightAttackEvent();
	void PerformLightAttack();
	void SaveLightAttack();
	void CheckCanAttack();

	void LaunchEvent();
	void LightLaunchAttack();
	void LightCrashAttack();
	void PerformLaunchAttack();
	void PerformCrashAttack();


	//Heavy Attack
	void HeavyAttackInput();
	void PerformHeavyAttack();
	void SaveHeavyAttack();

	void MusouAttackInput();
	void PefromMusouAttack();
	void SaveMusouAttack();

	//Dodge
	void DodgeInput();
	void PerformDodge();
	bool CheckCanDodge();
	void SaveDodge();

	//Jump
	void PerformJump();
	virtual void Landed(const FHitResult& Hit) override;

	//LockOn
	void LockOnInput();
	void PerformLockOn(float DeltaTime);
	void SoftLockOnCheck();
	void PerformSoftLockOn();

	//ETC
	void Init();
	void ResetState();
	void CheckAttack();
	bool CheckInRadialRange(AActor* Target);

	void MouseInputCheck(float Value);
	void ShowAttackRange(float Radius);

	void RemoveOverlapArrayDuplicates(TArray<FOverlapResult>& Array);

private:
	UPROPERTY()
		bool bIsCanAttack;

	UPROPERTY()
		bool bSaveLightAttack;

	UPROPERTY()
		bool bPerformLaunchAttack;

	UPROPERTY()
		int CurLightAtkIndex;

	UPROPERTY()
		int MaxLightAtk;

	UPROPERTY()
		int CurAirAtkIndex;

	UPROPERTY()
		int MaxAirAtk;

	UPROPERTY()
		bool bSaveHeavyAttack;

	UPROPERTY()
		bool bSaveDodge;

	UPROPERTY()
		bool bDoubleJumped;

	UPROPERTY()
		bool bJustJumped;

	UPROPERTY()
		bool bTargeting;

	UPROPERTY(VisibleAnywhere, Category = Targeting)
		AActor* TargetActor;

	UPROPERTY(VisibleAnywhere, Category = Targeting)
		AActor* SoftTargetActor;

	UPROPERTY(VisibleAnywhere, Category = State)
		E_PlayerState CurPlayerState;

	UPROPERTY()
		class UDYAnimInstance* DYAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge, meta = (AllowPrivateAccess = "true"))
		float DodgeVal = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump, meta = (AllowPrivateAccess = "true"))
		float DoubleJumpFwdVal = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump, meta = (AllowPrivateAccess = "true"))
		float DoubleJumpUpVal = 500.f;

	UPROPERTY();
	FTimerHandle Handle;

	UPROPERTY()
	UBYDTMain* CurDamageType;

	bool bisAttackClicked = false;

	FVector LaunchVelocity;
	FVector GroundLocation;

	bool bMousePressed = false;
	float PressedTime = 0.f;

	//LockOn시 target위치에 따라 z값 조절
	FVector AirVector = FVector(0.f, 0.f, 80.f);
	FVector GroundVector = FVector(0.f, 0.f, 30.f);

};


