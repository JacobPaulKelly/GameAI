#include "AI/StateMachine/AttackingState.h"
#include "AI/StateMachine/SearchingState.h"
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PawnMovementComponent.h"

AttackingState::AttackingState()
{
	FireRate = new float(5); // how many bullets a second.
	Cooldown = new float (0);
	Accuracy = new FVector(FMath::RandRange(-AccuracyOffset, AccuracyOffset));
	Damage = new float(5);
	AimDirection = new FVector(0,0,0);
	TracelineLength = new float (5000.0f);
}

AttackingState::~AttackingState()
{
	delete FireRate;
	FireRate = nullptr;
	delete Accuracy;
	Accuracy = nullptr;
	delete Damage;
	Damage = nullptr;
	delete AimDirection;
	AimDirection = nullptr;
	delete TracelineLength;
	TracelineLength = nullptr;
}

void AttackingState::Setup(AAICharacter* Ai)
{
	MovementComponent = Ai->GetMovementComponent();
}

void AttackingState::Execute(AAICharacter* Ai, float DeltaTime)
{
	if (Ai->Enemies.empty())
	{
		Ai->ChangeAiState(new SearchingState);
		return;
	}

	for (int32 i = 0; i < Ai->Enemies.size(); ++i)
	{
		if (Ai->Enemies[i]->GetIsDead() || Ai->Enemies[i]->GetCurrentHealth() <= 0)
		{
			// Set the flag to false if any enemy is dead
			Ai->ChangeAiState(new SearchingState);
			
			return; // No need to check further, we already know not all enemies are alive
		}
	}
	
	
	float ClosestDistance = 999999999.0f;
	FVector TempLocation;
	
	for (int32 i = 0; i < Ai->Enemies.size(); ++i)
	{
		if (Ai->Enemies[i]->GetCurrentHealth()>0) //was trying to use getisdead but for some reason produced problems.
		{
			TempLocation = Ai->Enemies[i]->GetActorLocation() - Ai->GetActorLocation();
			
			if (TempLocation.SizeSquared() < ClosestDistance)
			{
				ClosestDistance = TempLocation.SizeSquared();
				
				*AimDirection = AimDirectionWithAccuracy(TempLocation);
			}
		}
	}
	
	// float ClosestDistance = 999999999;
	//
	// for (int32 i = 0; i < Ai->Enemies.size(); ++i)
	// {
	// 	TempLocation = Ai->Enemies[i]->GetActorLocation() - Ai->GetActorLocation();
	//    
	// 	if (!Ai->Enemies[i]->GetIsDead()) // bIsDead = false then do the thing
	// 	{
	// 		float CurrentDistance = TempLocation.SizeSquared();
	//
	// 		if (CurrentDistance < ClosestDistance)
	// 		{
	// 			ClosestDistance = CurrentDistance;
	// 			*AimDirection = AimDirectionWithAccuracy(TempLocation);
	// 		}
	// 	}
	// }

	FVector Direction = AimDirection->GetSafeNormal2D();

	MovementComponent->AddInputVector(Direction*DeltaTime);

	Ai->SetActorRotation(AimDirection->Rotation());

	*Cooldown += DeltaTime * *FireRate;

	if (*Cooldown >= 1)
	{
		LineTrace(Ai); //shoot gun

		*Cooldown = 0.0f;
	}
}

FVector AttackingState::AimDirectionWithAccuracy(FVector PreviousDirection)
{
	FVector NewDirection = PreviousDirection.GetSafeNormal();
	NewDirection += *Accuracy;
	*Accuracy = FVector(FMath::RandRange(-AccuracyOffset, AccuracyOffset));
	
	return NewDirection;
}

void AttackingState::LineTrace(AAICharacter* AI)
{
	UWorld* world = AI->GetWorld();
	
	// Get the actor's location and rotation
	FVector StartLocation = AI->GetActorLocation();
	//FRotator ActorRotation = AI->GetActorRotation();

	// Calculate the end location based on the direction and length
	//FVector EndLocation = StartLocation + ActorRotation.Vector() * *TracelineLength;
	FVector EndLocation = StartLocation + *AimDirection * *TracelineLength;

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

			HitActor->DamageHealth(AI, *Damage, StartLocation);
		}
		
		// Access information about the hit result (e.g., HitResult.Actor, HitResult.Location, etc.)
	}

	DrawDebugLine(world,StartLocation, EndLocation, FColor::Green, false, 2.0f, 0, 2.0f);
}