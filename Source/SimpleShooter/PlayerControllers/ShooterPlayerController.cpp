// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetUpHUD();
}

void AShooterPlayerController::SetUpHUD()
{
	HUD = CreateWidget(this, HUDClass);
	if (HUD)
	{
		HUD->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No HUDClass set for %s."), *GetName());
	}
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HandleGameOver(bIsWinner);
}


void AShooterPlayerController::SetUpNewGame()
{
	RestartLevel();
}

void AShooterPlayerController::HandleGameOver(bool bPlayerWon)
{
	if (!HUD) { return; }
	HUD->RemoveFromViewport();
	GameOver(bPlayerWon);
}


