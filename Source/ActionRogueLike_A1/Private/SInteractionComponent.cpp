// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Distance = 1000.0f;
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
	FVector End = EyeLocation + EyeRotation.Vector() * Distance;

	FCollisionShape shape;
	shape.SetSphere(20.f);

	TArray<FHitResult> hits;
	GetWorld()->SweepMultiByObjectType(hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, shape);

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

