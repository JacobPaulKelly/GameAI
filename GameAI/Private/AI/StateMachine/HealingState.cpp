#include "AI/StateMachine/HealingState.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PawnMovementComponent.h"

HealingState::HealingState()
{
	LowestHealth = 9999.0f;
	Destination = FVector::Zero();
	SlowStrength = 1.0f;
	Accuracy = FVector::Zero();
	Cooldown = 0;
	DirectionVelocity = FVector::Zero();
}

HealingState::~HealingState()
{
}

void HealingState::Setup(AAICharacter* Ai)
{
	MovementComponent = Ai->GetMovementComponent();
}

void HealingState::Execute(AAICharacter* Ai, float DeltaTime)
{
	Ai->UpdateTeamList();
	
	for (int i = 0; i < Ai->TeamMates.size(); ++i)
	{
		//itterate though and find team mate with lowest health
		
		// if (Ai->TeamMates[i]->GetIsDead())
		// {
		// 	Ai->RemoveFromTeamList(Ai->TeamMates[i]);
		// 	return;
		// }
		if (!Ai->TeamMates[i])
		{
			continue; // if this target doesnt exist move on to next itteration
		}
		if (Ai->TeamMates[i]->GetCurrentHealth() < LowestHealth && !Ai->TeamMates[i]->GetIsDead())
		{
			LowestHealth = Ai->TeamMates[i]->GetCurrentHealth();

			Target = Ai->TeamMates[i];
		}
	}
	LowestHealth = 9999.0f; //reseting to make sure medic is always at lowest health target

	if (Target && Ai)
	{
		Destination = Target->GetActorLocation() - Ai->GetActorLocation();
		
		DirectionVelocity = Destination.GetSafeNormal2D() * Ai->GetMaxSpeed();

		if (Destination.Size() < SlowDistanceMax)
		{
			//UE_LOG(LogTemp,Warning, TEXT(" within range"));
		
			SlowStrength = (Destination.Size() - SlowDistanceMin) / (SlowDistanceMax - SlowDistanceMin);
		
			Cooldown += DeltaTime * FireRate;

			//UE_LOG(LogTemp,Warning, TEXT("%f"), Cooldown);

			if (Cooldown >= 1)
			{
				LineTrace(Ai);

				Cooldown = 0;
			}
		
			DirectionVelocity = (Destination.GetSafeNormal2D() * Ai->GetMaxSpeed()) * (SlowStrength);		
		}

		MovementComponent->AddInputVector(DirectionVelocity);

		Ai->SetActorRotation(DirectionVelocity.Rotation());
	}
}

void HealingState::LineTrace(AAICharacter* AI)
{
	UWorld* world = AI->GetWorld();
	
	// Get the actor's location and rotation
	FVector StartLocation = AI->GetActorLocation();
	//FRotator ActorRotation = AI->GetActorRotation();

	FVector AimDirection = AimDirectionWithAccuracy(Destination);

	// Calculate the end location based on the direction and length
	//FVector EndLocation = StartLocation + ActorRotation.Vector() * *TracelineLength;
	FVector EndLocation = StartLocation + AimDirection * TracelineLength;

	// Set the collision channel to use for the line trace
	ECollisionChannel TraceChannel = ECC_Visibility;

	// Configure the trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("LineTraceByChannel")), false, AI);

	// Perform the line trace
	FHitResult HitResult;
	bool bHit = world->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, TraceChannel, TraceParams);

	// Check if the line trace hit something
	if (bHit)
	{
		if(Cast<AAICharacter>(HitResult.GetActor()))
		{
			AAICharacter* HitActor = Cast<AAICharacter>(HitResult.GetActor());

			HitActor->HealDamage(HealAmmount);
		}
		
		// Access information about the hit result (e.g., HitResult.Actor, HitResult.Location, etc.)
	}

	DrawDebugLine(world,StartLocation, EndLocation, FColor::Blue, false, 2.0f, 0, 2.0f);
}

FVector HealingState::AimDirectionWithAccuracy(FVector OriginalAim)
{
	FVector NewDirection = OriginalAim.GetSafeNormal();
	NewDirection += Accuracy;
	Accuracy = FVector(FMath::RandRange(-AccuracyOffset, AccuracyOffset));
	
	return NewDirection;
}
