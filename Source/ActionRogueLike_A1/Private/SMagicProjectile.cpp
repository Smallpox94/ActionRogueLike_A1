// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	//Collision Setup
	SphereComp->SetCollisionProfileName("Projectile");

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	ProjectileMovementComp->InitialSpeed = 1000.0f;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;


}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnHitCallback);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASMagicProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SphereComp->OnComponentHit.RemoveDynamic(this, &ASMagicProjectile::OnHitCallback);
}

void ASMagicProjectile::OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetInstigator())
	{
		//Add FX at destroy
		FTransform spawnTransform;
		spawnTransform.SetLocation(GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFXParticleSystem, spawnTransform);
		
		//Add Impulse
		if (OtherActor->IsRootComponentMovable())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, Hit.ImpactPoint);
		}
		Destroy();
	}
}

