#include "DYEnemyController.h"
#include "Engine/DamageEvents.h"
#include "BYDTMain.h"
#include "DYEnemyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"


ADYEnemyController::ADYEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADYEnemyController::BeginPlay()
{
	Super::BeginPlay();
	CurState = E_EnemyState::Idle;
}

/// <summary>
/// 데미지 입을시 플레이어 방향 바라보게 하기
/// </summary>
void ADYEnemyController::RotateWhenDamage(FVector CauserLocation)
{
	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CauserLocation);
	LookRot.Pitch = GetActorRotation().Pitch;

	SetActorRotation(LookRot);
}

/// <summary>
/// 캐릭터 Launch 시키기
/// </summary>
void ADYEnemyController::LaunchingCharacter()
{
	Buffer(-5.0f);
	CurState = E_EnemyState::Launch;

	FVector DesiredLocation = GetActorLocation();
	DesiredLocation.Z += LaunchRange;

	SetActorLocation(UKismetMathLibrary::VLerp(GetActorLocation(), DesiredLocation, 0.5f));
}

/// <summary>
/// 공중 콤보 공격시 캐릭터 공중에 launch 시키기
/// </summary>
/// <param name="CauserLocation"></param>
void ADYEnemyController::LaunchWhenAirAttack(FVector CauserLocation)
{
	FVector LauncedVector = FVector(GetActorLocation().X, GetActorLocation().Y, CauserLocation.Z+250.f);

	CurState = E_EnemyState::Launch;
	SetActorLocation(UKismetMathLibrary::VLerp(GetActorLocation(), LauncedVector, 0.5f));
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void ADYEnemyController::KnockDownCrasher()
{
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

		SetActorLocation(UKismetMathLibrary::VLerp(GetActorLocation(), GroundLocation, 1.f), true);
	}
}

void ADYEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ADYEnemyController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAnim = Cast<UDYEnemyAnimInstance>(GetMesh()->GetAnimInstance());


	EnemyAnim->OnEnemyFall.AddWeakLambda(this, [this]() { GetCharacterMovement()->SetMovementMode(MOVE_Falling); });
	EnemyAnim->OnEnemyRecover.AddWeakLambda(this, [this]() { CurState = E_EnemyState::Idle; });
}

/// <summary>
/// 피격시 호출
/// </summary>
/// <param name="DamageAmount">데미지 량</param>
/// <param name="DamageEvent">데미지 타입</param>
/// <param name="EventInstigator"></param>
/// <param name="DamageCauser">데미지 유발자</param>
void ADYEnemyController::TakeDamage(float DamageAmount, UDamageType* DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UBYDTMain* DamageType = Cast<UBYDTMain>(DamageEvent);
	RotateWhenDamage(DamageCauser->GetActorLocation());

	MoveWhenDamaged(DamageType, DamageCauser->GetActorLocation());

	EnemyAnim->CheckHitAnimation(DamageType->GetDamageType());

}


/// <summary>
/// 데미지 타입마다 움직임 로직 처리
/// </summary>
/// <param name="DamageType"></param>
/// <param name="CauserLocation"></param>
void ADYEnemyController::MoveWhenDamaged(UBYDTMain *DamageType, FVector CauserLocation)
{
	switch (DamageType->GetDamageType())
	{
	case E_DmgType::DT_Launch:
		Buffer(-3.f);
		LaunchRange = DamageType->MoveValue;
		LaunchingCharacter();
		break;

	case E_DmgType::DT_AirATK:
		Buffer(-1.f);
		LaunchWhenAirAttack(CauserLocation);
		break;

	case E_DmgType::DT_KnockBack:
		Buffer(-5.f);
		break;

	case E_DmgType::DT_KnockDown:
		KnockDownCrasher();
		break;

	case E_DmgType::DT_Light:
		Buffer(-5.f);
		break;


	case E_DmgType::Nothing:
	default:
		break;
	}

}






