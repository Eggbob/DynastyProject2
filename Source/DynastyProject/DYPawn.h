// Class Default PawnClass

#pragma once

#include "EngineMinimal.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "DYPawn.generated.h"

UCLASS()
class DYNASTYPROJECT_API ADYPawn : public APawn
{
	GENERATED_BODY()

public:
	ADYPawn();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;


public:
	UPROPERTY(VisibleAnywhere, Category = Collision)
		UCapsuleComponent* PawnCapsule;

	UPROPERTY(VisibleAnywhere, Category = Visual)
		USkeletalMeshComponent* PawnMesh;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UFloatingPawnMovement* PawnMovement;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* PawnSpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* PawnCamera;

private:
	void UpDown(float AxisValue);

	void LeftRight(float AxisValue);
	
	void InitPawn();
};
