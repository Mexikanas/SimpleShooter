// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

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

	if (!MuzzleFlash)
	{
		UE_LOG(LogTemp, Error, TEXT("No Particle effect has been attached to the %s actor"), *GetOwner()->GetName());
	}
	
}

void AGun::PullTriggerMethod()
{
	if (!MuzzleFlash) { return; }
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

