// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include <Camera/CameraComponent.h>
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Distance = 250.0f;
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	//FVector End = EyeLocation + EyeRotation.Vector() * Distance;

	FVector Start = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()))->GetComponentLocation();
	Start = EyeLocation;
	FVector End = GetOwner()->GetInstigatorController()->GetControlRotation().Vector() * 100.0f + EyeLocation;

	FCollisionShape shape;
	shape.SetSphere(30.f);

	TArray<FHitResult> hits;
	GetWorld()->SweepMultiByObjectType(hits, Start, End, FQuat::Identity, ObjectQueryParams, shape);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 4.0f, 0 ,3.0f);
	for (FHitResult& hit : hits)
	{
		AActor* hitActor = hit.GetActor();
		if (hitActor && hitActor->Implements<USGameplayInterface>())
		{
			APawn* pawn = Cast<APawn>(GetOwner());
			ISGameplayInterface::Execute_Interact(hitActor, pawn);
		}
	}
}

