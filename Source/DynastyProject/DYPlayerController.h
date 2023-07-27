// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DYPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DYNASTYPROJECT_API ADYPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;


protected:
	virtual void BeginPlay() override;
};
