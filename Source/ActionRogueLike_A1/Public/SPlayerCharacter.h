// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_A1_API ASPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASPlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere)
	UAnimMontage* PrimaryAttackAnimMontage;

	FTimerHandle TimerHandle_PrimaryAttack;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void Jump();
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void PrimaryInteract();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileTemplate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
