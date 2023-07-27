#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "BYDTMain.h"
#include "DYEnemyController.generated.h"


UENUM(BlueprintType)
enum class E_EnemyState : uint8
{
	Nothing UMETA(DisplayName = "Nothing"),
	Idle UMETA(DisplayName = "Idle"),
	Attack UMETA(DisplayName = "Attack"),
	Move UMETA(DisplayName = "Move"),
	Defense UMETA(DisplayName = "Defense"),
	Hit UMETA(DisplayName = "Hit"),
	Launch UMETA(DisplayName = "Launch"),
	Down UMETA(DisplayName = "Down"),
	Die UMETA(DisplayName = "Die")
};

UCLASS()
class DYNASTYPROJECT_API ADYEnemyController : public ACharacter
{
	GENERATED_BODY()

public:
	ADYEnemyController();

protected:
	virtual void BeginPlay() override;


private:
	void RotateWhenDamage(FVector CauserLocation);

	void LaunchingCharacter();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	void TakeDamage(float DamageAmount, class UDamageType* DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, Category = Buffer)
		void Buffer(float BufferAmount);

	UFUNCTION(BlueprintImplementableEvent, Category = Buffer)
		void StopBuffer();

private:
	void MoveWhenDamaged(UBYDTMain *DamageType, FVector CauserLocation);

	void LaunchWhenAirAttack(FVector CauserLocation);

	void KnockDownCrasher();

private:
	UPROPERTY()
		class UDYEnemyAnimInstance* EnemyAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float LaunchRange = 700.f;

	UPROPERTY()
		E_EnemyState CurState;

	FVector GroundLocation;

};
