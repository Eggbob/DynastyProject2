// Project Default Pawn Class

#include "DYPawn.h"

ADYPawn::ADYPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	InitPawn();
}

void ADYPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADYPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADYPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ADYPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ADYPawn::LeftRight);
}

void ADYPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ADYPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

/// <summary>
/// ���� �̵�
/// </summary>
/// <param name="AxisValue"></param>
void ADYPawn::UpDown(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

/// <summary>
/// �¿� �̵�
/// </summary>
/// <param name="AxisValue"></param>
void ADYPawn::LeftRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

/// <summary>
/// Pawn �ʱ� ����
/// </summary>
void ADYPawn::InitPawn()
{
	//�ʼ� ������Ʈ ����
	PawnCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	PawnMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	PawnSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	//��Ʈ ���� �� ������Ʈ ��ġ ����
	RootComponent = PawnCapsule;
	PawnMesh->SetupAttachment(PawnCapsule);
	PawnSpringArm->SetupAttachment(PawnCapsule);
	PawnCamera->SetupAttachment(PawnSpringArm);

	//������Ʈ �ʱ� ����
	PawnCapsule->SetCapsuleHalfHeight(88.f);
	PawnCapsule->SetCapsuleRadius(35.f);
	PawnMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	PawnSpringArm->TargetArmLength = 400.f;
	PawnSpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	//�� Mesh ����
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Kwang(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC'"));

	//if (SK_Kwang.Succeeded())
	//{
	//	PawnMesh->SetSkeletalMesh(SK_Kwang.Object);
	//}

	//�� �ִϸ��̼� ����
	PawnMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_GreateSword
	(TEXT("/Script/Engine.AnimBlueprint'/Game/Assets/Animations/ABP_GreateSwordAnim.ABP_GreateSwordAnim_C'"));

	if (Anim_GreateSword.Succeeded()) 
	{ 
		PawnMesh->SetAnimInstanceClass(Anim_GreateSword.Class); 
	}
	else 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Error"));
	}
}



