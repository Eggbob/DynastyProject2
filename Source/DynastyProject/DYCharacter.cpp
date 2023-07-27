#include "DYCharacter.h"
#include "DYAnimInstance.h"
#include "DynastyProjectGameMode.h"
#include "DrawDebugHelpers.h"
#include "DYEnemyController.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"




ADYCharacter::ADYCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Init();
}

void ADYCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ADYCharacter::Init()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 450.f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->JumpZVelocity = 800.f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Kwang(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_SKM.Kwang_SKM'"));

	if (SK_Kwang.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Kwang.Object);
	}

	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_GreateSword
	(TEXT("/Script/Engine.AnimBlueprint'/Game/Assets/Animations/ABP_GreateSwordAnim.ABP_GreateSwordAnim_C'"));

	if (Anim_GreateSword.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_GreateSword.Class);
	}

}

void ADYCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PerformLockOn(DeltaTime);
}

void ADYCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindCharacterAxis(PlayerInputComponent);
}

void ADYCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();


	CurDamageType = NewObject<UBYDTMain>();

	CurLightAtkIndex = 0;
	MaxLightAtk = 3;
	CurAirAtkIndex = 0;
	MaxAirAtk = 4;

	CurPlayerState = E_PlayerState::Nothing;
	bIsCanAttack = true;
	bSaveLightAttack = false;
	bSaveDodge = false;
	bDoubleJumped = false;
	bJustJumped = false;

}

void ADYCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DYAnim = Cast<UDYAnimInstance>(GetMesh()->GetAnimInstance());

	//Save Attack Lamda 
	DYAnim->OnCheckCombo.AddUObject(this, &ADYCharacter::SaveLightAttack);
	DYAnim->OnSaveDodge.AddUObject(this, &ADYCharacter::SaveDodge);
	DYAnim->OnResetState.AddUObject(this, &ADYCharacter::ResetState);
	DYAnim->OnAnimMove.AddUObject(this, &ADYCharacter::MoveDuringAnim);
	DYAnim->OnAttackCheck.AddUObject(this, &ADYCharacter::CheckAttack);
	DYAnim->OnLaunch.AddUObject(this, &ADYCharacter::PerformLaunchAttack);
	DYAnim->OnCrash.AddUObject(this, &ADYCharacter::PerformCrashAttack);
	DYAnim->OnLightLuanch.AddUObject(this, &ADYCharacter::LightLaunchAttack);
	DYAnim->OnLightCrash.AddUObject(this, &ADYCharacter::LightCrashAttack);
}


void ADYCharacter::BindCharacterAxis(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ADYCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ADYCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ADYCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LeftMouseInput"), this, &ADYCharacter::MouseInputCheck);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADYCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this , &ADYCharacter::PerformJump);

	PlayerInputComponent->BindAction(TEXT("HeavyAttack"), EInputEvent::IE_Pressed, this ,&ADYCharacter::HeavyAttackInput);
	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this ,&ADYCharacter::DodgeInput);
	PlayerInputComponent->BindAction(TEXT("MusouAttack"), EInputEvent::IE_Pressed, this ,&ADYCharacter::MusouAttackInput);
	PlayerInputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this ,&ADYCharacter::LockOnInput);
	
}


E_PlayerState ADYCharacter::GetCurrentPlayerState() const
{
	return CurPlayerState;
}

float ADYCharacter::GetAnimMoveSpeed()
{
	float speed = 0.f;

	switch (ADYCharacter::GetCurrentPlayerState())
	{
	case E_PlayerState::LightAttack:
		speed = 5.f;
		break;

	case E_PlayerState::HeavyAttack:
		speed = 10.f;
		break;
	}

	return speed;
}

void ADYCharacter::MouseInputCheck(float Value)
{
	if (Value > 0)
	{
		if (!bMousePressed)
		{
			PressedTime = GetWorld()->TimeSeconds;
		}
		else
		{
			if (PressedTime + 0.3f <= GetWorld()->TimeSeconds)
			{
				bSaveDodge = false; 
				bSaveHeavyAttack = false;

				LaunchEvent();
			}
		}
			bMousePressed = true;
	}
	else
	{
		if (bMousePressed)
		{
			if (PressedTime + 0.1f > GetWorld()->TimeSeconds)
			{
				//UE_LOG(LogTemp, Warning, TEXT("PressedTime : %f"), PressedTime + 0.01f);
				//UE_LOG(LogTemp, Warning, TEXT("WorldTime : %f"), GetWorld()->TimeSeconds);
				LightAttackEvent();
			}
		}

		bMousePressed = false;
		PressedTime = 0.f;
	}
}


#pragma region MoveMent

void ADYCharacter::UpDown(float AxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), AxisValue);
}

void ADYCharacter::LeftRight(float AxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), AxisValue);
}

void ADYCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ADYCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

/// <summary>
/// 점프 수행
/// </summary>
void ADYCharacter::PerformJump()
{
	if (CurPlayerState == E_PlayerState::LightAttack || CurPlayerState == E_PlayerState::Dodge || CurPlayerState == E_PlayerState::HeavyAttack) { return; }

	if (!GetCharacterMovement()->IsFalling())
	{
		bJustJumped = true;
		Jump();

		FTimerHandle WaitHandle;

		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]() 
		{
				bJustJumped = false;
		}), 0.5f, false);
	}
	else //더블 점프
	{
		if (!bDoubleJumped)
		{
			bJustJumped = false;
			bDoubleJumped = true;
			LaunchVelocity = GetActorForwardVector() * DoubleJumpFwdVal;

			LaunchVelocity.Z = DoubleJumpUpVal;

			DYAnim->PlayDoubleJumpMontage();

			LaunchCharacter(LaunchVelocity, false, true);
		}
	}
	
}

/// <summary>
/// 착지시에 
/// </summary>
/// <param name="Hit"></param>
void ADYCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (Hit.GetComponent()->GetClass() == UStaticMeshComponent::StaticClass())
	{
		CurAirAtkIndex = 0;
	
		bPerformLaunchAttack = false;
		bDoubleJumped = false;
		bJustJumped = false;
	}
}


#pragma endregion

#pragma region LockOn
void ADYCharacter::LockOnInput()
{
	if (!bTargeting && TargetActor == nullptr)
	{
		FVector EndVector = (Camera->GetForwardVector() * 600.f) + GetActorLocation();
		TArray<AActor*> IgnoreActor;
		IgnoreActor.Add(this);

		FHitResult HitResult;

		const bool IsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), EndVector, 100.f,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2), false, IgnoreActor,
			EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Yellow, FLinearColor::Green, 5.f);

		if (IsHit && HitResult.GetActor()->IsValidLowLevel())
		{
			bTargeting = true;
			TargetActor = HitResult.GetActor();
		}
	}
	else
	{
		bTargeting = false;
		TargetActor = nullptr;
	}
}

/// <summary>
/// LockOn상태일때 수행
/// </summary>
/// <param name="DeltaTime"></param>
void ADYCharacter::PerformLockOn(float DeltaTime)
{
	if (bTargeting && TargetActor != nullptr)
	{
		if (GetDistanceTo(TargetActor) <= 1000.f)
		{
			ACharacter* target = Cast<ACharacter>(TargetActor);
			FVector LookVector = (target->GetCharacterMovement()->IsFalling() || target->GetCharacterMovement()->IsFlying()) ? (TargetActor->GetActorLocation() - AirVector) : (TargetActor->GetActorLocation() - GroundVector);
			FRotator LookRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookVector);

			GetController()->SetControlRotation(UKismetMathLibrary::RInterpTo(GetControlRotation(), LookRotator, DeltaTime, 5.f));
		}
		else
		{
			bTargeting = false;
			TargetActor = nullptr;
		}
	}
}

/// <summary>
/// SoftLockOn Target설정
/// </summary>
void ADYCharacter::SoftLockOnCheck()
{
	if (!bTargeting && TargetActor == nullptr)
	{
		FVector EndVector = GetActorLocation() + (GetCharacterMovement()->GetLastInputVector() * 10000.f) ;
		TArray<AActor*> IgnoreActor;
		IgnoreActor.Add(this);

		FHitResult HitResult;

		const bool IsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), EndVector, 100.f,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2), false, IgnoreActor,
			EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.f);
		if (IsHit && HitResult.GetActor()->IsValidLowLevel())
		{
			SoftTargetActor = HitResult.GetActor();
		}
		else { SoftTargetActor = nullptr; }
	}
	else { SoftTargetActor = nullptr; }
}

/// <summary>
/// SoftLock 수행
/// </summary>
void ADYCharacter::PerformSoftLockOn()
{
	if (TargetActor != nullptr)
	{
		if (SoftTargetActor != nullptr)
		{
			FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
			FRotator TargetRot = FRotator(GetActorRotation().Pitch, LookRot.Yaw, LookRot.Roll);

			SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), TargetRot, 1.f, false));
		}
	}
	else
	{
		if (SoftTargetActor != nullptr)
		{
			FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SoftTargetActor->GetActorLocation());
			FRotator TargetRot = FRotator(GetActorRotation().Pitch, LookRot.Yaw, LookRot.Roll);

			SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), TargetRot, 1.f, false));
		}
	}

}

#pragma endregion


#pragma region LightAttack
/// <summary>
/// 일반공격 입력 감지
/// </summary>
void ADYCharacter::LightAttackEvent()
{
	if (CurPlayerState == E_PlayerState::Nothing)
	{
		PerformLightAttack();
	}
	else
	{
		bSaveLightAttack = true;
	}
}


/// <summary>
/// 일반공격 실행
/// </summary>
void ADYCharacter::PerformLightAttack()
{
	if (DYAnim == nullptr) return;

	CheckCanAttack();

	if (bIsCanAttack)
	{
		StopBuffer();
		Buffer(3.0f);
		CurPlayerState = E_PlayerState::LightAttack;
		SoftLockOnCheck();
		PerformSoftLockOn();
		
		CurDamageType->SetDamageType(E_DmgType::DT_Light);

		DYAnim->PlayLightAttackMontage(CurLightAtkIndex);

		CurLightAtkIndex = CurLightAtkIndex == MaxLightAtk  ? 0 : CurLightAtkIndex + 1;
	}
	else if(bPerformLaunchAttack && CurPlayerState == E_PlayerState::Nothing)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

		CurPlayerState = E_PlayerState::LightAttack;
		CurDamageType->SetDamageType(E_DmgType::DT_AirATK);
		
		StopBuffer();

		SoftLockOnCheck();
		PerformSoftLockOn();

		DYAnim->PlayerAirAttackMontage(CurAirAtkIndex);

		CurAirAtkIndex++;

		if (CurAirAtkIndex == MaxAirAtk)
		{
			bPerformLaunchAttack = false;
			CurAirAtkIndex = 0;
		}
	}
}

/// <summary>
/// 다음 공격 확인
/// </summary>
void ADYCharacter::SaveLightAttack()
{
	if (bSaveLightAttack)
	{
		bSaveLightAttack = false;

		if (CurPlayerState == E_PlayerState::LightAttack || CurPlayerState == E_PlayerState::LaunchAttack)
		{
			CurPlayerState = E_PlayerState::Nothing;
		}

		PerformLightAttack();
	}
}

/// <summary>
/// 공격 가능 유무
/// </summary>
void ADYCharacter::CheckCanAttack()
{
	bIsCanAttack = !GetMovementComponent()->IsFalling() && !GetMovementComponent()->IsFlying()
		&& CurPlayerState != E_PlayerState::Dodge && CurPlayerState != E_PlayerState::LightAttack && CurPlayerState != E_PlayerState::HeavyAttack;
}
#pragma endregion

#pragma region LaunchAttack

void ADYCharacter::LaunchEvent()
{
	CheckCanAttack();

	if (bIsCanAttack && bTargeting && !bPerformLaunchAttack)
	{
		bPerformLaunchAttack = true;

		CurPlayerState = E_PlayerState::LaunchAttack;
		CurDamageType->SetDamageType(E_DmgType::DT_Launch);
		CurDamageType->MoveValue = 700.f;

		DYAnim->PlayLaunchAttackMontage();
	}
	else
	{
		LightAttackEvent();
	}
}

/// <summary>
/// 일반공격시의 Launch Attack
/// </summary>
void ADYCharacter::LightLaunchAttack()
{
	CurDamageType->SetDamageType(E_DmgType::DT_Launch);
	CurDamageType->MoveValue = 400.f;
}


/// <summary>
/// Launch attack 시 플레이어 올리기
/// </summary>
void ADYCharacter::PerformLaunchAttack()
{
	FVector DesiredLocation = GetActorLocation();

	DesiredLocation.Z += 700.f;

	SetActorLocation(FMath::Lerp(GetActorLocation(), DesiredLocation, 1.f), true);
}

void ADYCharacter::LightCrashAttack()
{
	CurDamageType->SetDamageType(E_DmgType::DT_KnockDown);
}



void ADYCharacter::PerformCrashAttack()
{
	CurDamageType->SetDamageType(E_DmgType::DT_KnockDown);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	ObjectTypes.Add(WorldStatic);
	ObjectTypes.Add(WorldDynamic);

	TArray<AActor*> IgnoreActors;
	FHitResult HitResult;

	FVector EndLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 10000000.f);

	bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		GetActorLocation(),
		EndLocation,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (bResult)
	{
		GroundLocation = HitResult.ImpactPoint;
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	
	SetActorLocation(FMath::Lerp(GetActorLocation(), GroundLocation, 1.f), true);
}
#pragma endregion


#pragma region HeavyAttack

/// <summary>
/// Heavy Attack 입력
/// </summary>
void ADYCharacter::HeavyAttackInput()
{
	bSaveDodge = false;
	bSaveLightAttack = false;

	if (CurPlayerState == E_PlayerState::LightAttack || CurPlayerState == E_PlayerState::HeavyAttack || CurPlayerState == E_PlayerState::Dodge)
	{
		bSaveHeavyAttack = true;
	}
	else
	{
		PerformHeavyAttack();
	}
}

/// <summary>
/// Heavy Attack 재생
/// </summary>
void ADYCharacter::PerformHeavyAttack()
{
	CheckCanAttack();

	if (bIsCanAttack)
	{
		bSaveLightAttack = false;
		CurLightAtkIndex = 0;

		StopBuffer();
		Buffer(3.0f);

		CurPlayerState = E_PlayerState::HeavyAttack;
		SoftLockOnCheck();
		DYAnim->PlayHeavyAttackMontage();
	}
}

/// <summary>
/// 다음 Heavy Attack 감지
/// </summary>
void ADYCharacter::SaveHeavyAttack()
{
	if (bSaveHeavyAttack)
	{
		bSaveHeavyAttack = false;

		if (CurPlayerState == E_PlayerState::HeavyAttack || CurPlayerState == E_PlayerState::LightAttack)
		{
			CurPlayerState = E_PlayerState::Nothing;
		}

		PerformHeavyAttack();
	}
}
#pragma endregion

#pragma region Musou


void ADYCharacter::MusouAttackInput()
{

}

void ADYCharacter::PefromMusouAttack()
{

}

void ADYCharacter::SaveMusouAttack()
{

}

#pragma endregion

#pragma region Dodge

/// <summary>
/// Dodge입력
/// </summary>
void ADYCharacter::DodgeInput()
{
	if (CheckCanDodge())
	{
		PerformDodge();
	}
	else
	{
		bSaveDodge = true;
	}
}

/// <summary>
/// Dodge 수행
/// </summary>
void ADYCharacter::PerformDodge()
{
	if (GetLastMovementInputVector() != FVector::ZeroVector)
	{
		FRotator LastRotate = FRotationMatrix::MakeFromX(GetMovementComponent()->GetLastInputVector()).Rotator();
		LastRotate.Pitch = 0.f;

		SetActorRotation(LastRotate);

		StopBuffer();
		Buffer(20.f);

		CurPlayerState = E_PlayerState::Dodge;
		DodgeMoving();
		DYAnim->PlayDodgeMontage();
	}
}

/// <summary>
/// Dodge 가능 체크
/// </summary>
/// <returns></returns>
bool ADYCharacter::CheckCanDodge()
{
	return !GetCharacterMovement()->IsFalling() && CurPlayerState != E_PlayerState::Dodge;
}

/// <summary>
/// Dodge가 입력되었는지 체크
/// </summary>
void ADYCharacter::SaveDodge()
{
	if (!bSaveDodge) { return; }

	bSaveDodge = false;

	if (CurPlayerState == E_PlayerState::LightAttack)
	{
		CurPlayerState = E_PlayerState::Nothing;
	}

	PerformDodge();
}
#pragma endregion

void ADYCharacter::ResetState()
{
	if (GetMovementComponent()->IsFlying())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}

	CurPlayerState = E_PlayerState::Nothing;
	CurLightAtkIndex = 0;
	bSaveLightAttack = false;
	bSaveHeavyAttack = false;
	bSaveDodge = false;
	bPerformLaunchAttack = false;
}

/// <summary>
/// 공격 판정
/// </summary>
void ADYCharacter::CheckAttack()
{
	TArray<FOverlapResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		HitResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(500.f),
		Params
	);


	if (bResult)
	{
		RemoveOverlapArrayDuplicates(HitResults);
		for (FOverlapResult& Results : HitResults)
		{
			if (CheckInRadialRange(Results.GetActor()))
			{
				Cast<ADYEnemyController>(Results.GetActor())->TakeDamage(10.f, CurDamageType, GetController(), this);;
			}
		}
	}
	
#pragma region  Draw Sphere

	/*FVector TraceVec = GetActorForwardVector() * 500.f;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = 500.f * 0.5f + 300.f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = FColor::Red;
	float DebugLifeTime = 5.f;

	DrawDebugSphere(GetWorld(), GetActorLocation(), 500.f, 32, FColor::Red, false, 5.f);
	ShowAttackRange(120.f);*/
#pragma endregion
}

/// <summary>
/// 공격범위 안에 존재하는지
/// </summary>
/// <param name="_Target"></param>
/// <returns></returns>
bool ADYCharacter::CheckInRadialRange(AActor* Target)
{
	FVector FirstVector = GetActorForwardVector();
	FVector SecondVector = Target->GetActorLocation() - GetActorLocation();

	float DegreeBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FirstVector, SecondVector)));
	if (DegreeBetween <= 120 / 2.0f && Target->GetClass()->IsChildOf(ADYEnemyController::StaticClass()))
	{
		return true;
	}

	return false;
}



/// <summary>
/// 공격 범위 표시
/// </summary>
/// <param name="Radius"></param>
void ADYCharacter::ShowAttackRange(float Radius)
{
	float AngleInRadians = FMath::DegreesToRadians(Radius/2.f);
	float SinValue = FMath::Sin(AngleInRadians);
	float CosValue = FMath::Cos(AngleInRadians);

	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	FVector Dot1 = ForwardVector * CosValue + RightVector * SinValue;
	FVector Dot2 = ForwardVector * CosValue - RightVector * SinValue;
	Dot1.Normalize();
	Dot2.Normalize();

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + 1000.f * Dot1, FColor::Green, false, 2.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + 1000.f * Dot2, FColor::Blue, false, 2.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + 1000.f * ForwardVector, FColor::Red, false, 2.f);

}

//오버랩된 오브젝트중 중복되는 값이 있다면 제거
void ADYCharacter::RemoveOverlapArrayDuplicates(TArray<FOverlapResult>& Array)
{
	TArray<FOverlapResult> UniqueArray;
	TSet<AActor*> UniqueActors;

	for (const FOverlapResult& Result : Array)
	{
		AActor* OverlappedActor = Result.GetActor();

		if (OverlappedActor && !UniqueActors.Contains(OverlappedActor))
		{
			UniqueArray.Add(Result);
			UniqueActors.Add(OverlappedActor);
		}
	}

	Array = MoveTemp(UniqueArray);
}
