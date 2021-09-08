// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"


void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
		SetStartLocation();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AIBehavior not set in %s"), *GetPawn()->GetName());
	}
}

void AShooterAIController::SetStartLocation()
{
	GetBlackboardComponent()->SetValueAsVector
	(
		TEXT("StartLocation"),
		GetPawn()->GetActorLocation()
	);
}

bool AShooterAIController::IsDead() const
{
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	if (ControlledCharacter)
	{
		return ControlledCharacter->IsDead();
	}
	return true;
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}