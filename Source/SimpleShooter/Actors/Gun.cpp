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
		UE_LOG(LogTemp, Error, TEXT("No MuzzleFlash particle effect has been attached to the %s."), *GetName());
	}
	if (!ImpactParticle)
	{
		UE_LOG(LogTemp, Error, TEXT("No ImpactParticle effect has been attached to the %s."), *GetName());
	}
}

void AGun::PullTriggerMethod()
{
	if (!MuzzleFlash) { return; }
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	
	FHitResult HitResult;
	FVector ShotDirection;
	
	if (GunTrace(OUT HitResult, OUT ShotDirection))
	{
		if (!ImpactParticle) { return; }
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitResult.Location, ShotDirection.Rotation());
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			if (!OwnerController) { return; }
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (!OwnerController) { return false; }
	FVector PlayerLocation;
	FRotator PlayerRotation;
	OwnerController->GetPlayerViewPoint(OUT PlayerLocation,	OUT PlayerRotation);
	ShotDirection = -PlayerRotation.Vector();

	FVector LineEnd = PlayerLocation + PlayerRotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	
	//Channel for Bullet = GameTraceChannel1
	return GetWorld()->LineTraceSingleByChannel(OUT HitResult,	PlayerLocation, LineEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) { return nullptr; }
	return OwnerPawn->GetController();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

