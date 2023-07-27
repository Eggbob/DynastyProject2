// Fill out your copyright notice in the Description page of Project Settings.


#include "DYPlayerController.h"

void ADYPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ADYPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ADYPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//��ǲ ��� ���� ���߿� ����
	FInputModeGameOnly InputMod;
	SetInputMode(InputMod);
}
