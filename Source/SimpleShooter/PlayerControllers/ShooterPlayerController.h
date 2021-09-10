// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

class UUserWidget;

UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool bPlayerWon);

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
		int32 RestartDelay = 5;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
		void SetUpNewGame();

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(VisibleAnywhere)
		UUserWidget* HUD = nullptr;

	void HandleGameOver(bool bPlayerWon);
	void SetUpHUD();
};
