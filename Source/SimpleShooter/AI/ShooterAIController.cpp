// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

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

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	

}

