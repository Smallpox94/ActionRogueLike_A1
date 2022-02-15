// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASPlayerCharacter::ASPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
}

// Called when the game starts or when spawned
void ASPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASPlayerCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASPlayerCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASPlayerCharacter::Jump);
}

void ASPlayerCharacter::MoveForward(float value)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	AddMovementInput(controlRot.Vector(), value);
}

void ASPlayerCharacter::MoveRight(float value)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	FVector rightVector = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(rightVector, value);
}

void ASPlayerCharacter::Jump()
{
	ACharacter::Jump();
}

void ASPlayerCharacter::PrimaryAttack()
{
	PlayAnimMontage(PrimaryAttackAnimMontage);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASPlayerCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASPlayerCharacter::PrimaryAttack_TimeElapsed()
{
	// FVector SpawnLocation = GetActorLocation();
	FVector SpawnLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//Do a raycast from the Camera to the first collision object
	FHitResult outHit;
	FCollisionObjectQueryParams params(FCollisionObjectQueryParams::InitType::AllObjects);
	FCollisionQueryParams params2;
	params2.AddIgnoredActor(GetOwner());
	params2.AddIgnoredActor(GetInstigator());	
	FVector startTrace = CameraComp->GetComponentLocation();
	FVector endTrace = GetControlRotation().Vector() * 1000.f + startTrace; //Camera Location
	//DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Red, false, 5.0f);
	FRotator SpawnRotation = (endTrace - SpawnLocation).Rotation();

	if (GetWorld()->LineTraceSingleByObjectType(outHit, startTrace, endTrace, params, params2))
	{
		AActor* hitActor = outHit.GetActor();
		auto name = hitActor->GetName();
		//Compute the reverse rotation to spawn with the good rotation
		SpawnRotation = (outHit.ImpactPoint - SpawnLocation).Rotation();
	}
	GetWorld()->SpawnActor<AActor>(ProjectileTemplate, SpawnLocation, SpawnRotation, spawnParams);
}

void ASPlayerCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}
