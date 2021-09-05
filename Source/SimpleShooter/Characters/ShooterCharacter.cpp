// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "SimpleShooter/Actors/Gun.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	

	SpawnGun();
}

void AShooterCharacter::SpawnGun()
{
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}


// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AShooterCharacter::RunPressed);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AShooterCharacter::RunReleased);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	
	// controller input
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!GetOwner()) { return 0.f; }
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health is %f"), Health);

	if (Health == 0)
	{ 
		bShotFromBehind = IsPlayerBehindEnemy(DamageCauser);
		bDead = true;
	}

	if (IsDead())
	{
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	return DamageToApply;
}

bool AShooterCharacter::IsPlayerBehindEnemy(AActor* Shooter) const
{
	FVector ShooterLocation = Shooter->GetActorLocation();
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FVector FromOwnerToShooter = ShooterLocation - OwnerLocation;
	float AngleFromFront = FMath::Abs(FromOwnerToShooter.Rotation().Yaw - GetActorRotation().Yaw);

	if (AngleFromFront > 90.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AShooterCharacter::IsDead() const
{
	return bDead;
}

bool AShooterCharacter::IsShotFromBehind() const
{
	return bShotFromBehind;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	if (!bRun) { AxisValue *= WalkSpeed; }
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	if (!bRun) { AxisValue *= WalkSpeed; }
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::RunPressed()
{
	bRun = true;
}

void AShooterCharacter::RunReleased()
{
	bRun = false;
}

void AShooterCharacter::Shoot()
{
	if (!Gun) { return; }
	Gun->PullTriggerMethod();
}


