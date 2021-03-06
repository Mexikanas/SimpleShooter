// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"


UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTriggerMethod();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	void CheckForGunAssets() const;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash = nullptr;
	UPROPERTY(EditAnywhere)
		USoundBase* MuzzleSound = nullptr;
	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactParticle = nullptr;
	UPROPERTY(EditAnywhere)
		USoundBase* ImpactSound = nullptr;
	UPROPERTY(EditAnywhere)
		float MaxRange = 1000.f;
	UPROPERTY(EditAnywhere)
		float Damage = 10.f;

	bool GunTrace(FHitResult& HitResult, FVector& ShotDirection);

	AController* GetOwnerController() const;
};
