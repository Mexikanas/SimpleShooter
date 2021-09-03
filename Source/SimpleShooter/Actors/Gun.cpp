// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	CheckForMuzzleFlash();
}

void AGun::CheckForMuzzleFlash() const
{
	if (!MuzzleFlash)
	{
		UE_LOG(LogTemp, Error, TEXT("No MuzzleFlash particle effect has been attached to the %s."), *GetOwner()->GetName());
	}
}

void AGun::PullTriggerMethod()
{
	if (!MuzzleFlash) { return; }
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) { return; }
	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController) { return; }

	FVector PlayerLocation;
	FRotator PlayerRotation;
	OwnerController->GetPlayerViewPoint
	(
		OUT PlayerLocation,
		OUT PlayerRotation
	);

	FVector LineEnd = PlayerLocation + PlayerRotation.Vector() * MaxRange;
	FHitResult HitResult;

	bool bSuccess = GetWorld()->LineTraceSingleByChannel
	(
		OUT HitResult,
		PlayerLocation,
		LineEnd,
		//Channel for Bullet = GameTraceChannel1
		ECollisionChannel::ECC_GameTraceChannel1
	);

	if (bSuccess)
	{
		if (!ImpactParticle) { return; }
		FVector ShotDirection = -PlayerRotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			ImpactParticle,
			HitResult.Location,
			ShotDirection.Rotation()
		);
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitActor->TakeDamage
			(
				Damage,
				DamageEvent,
				OwnerController,
				this
			);
		}
	}
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

