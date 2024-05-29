#include "AI/StateMachine/ReturnToBattle.h"

#include "AI/StateMachine/HealingState.h"
#include "AI/StateMachine/SearchingState.h"
#include "GameFramework/PawnMovementComponent.h"

ReturnToBattle::ReturnToBattle()
{
	Destination = new FVector(0,0,0);
}

ReturnToBattle::~ReturnToBattle()
{
	delete Destination;
	Destination = nullptr;
}

void ReturnToBattle::Setup(AAICharacter* Ai)
{
	*Destination = FleeDestination(Ai,Ai->DamageLocation);
	MovementComponent = Ai->GetMovementComponent();
	Ai->DamageLocation = FVector::Zero();
}

void ReturnToBattle::Execute(AAICharacter* Ai, float DeltaTime)
{
	FVector Direction =  *Destination - FVector(Ai->GetActorLocation().X,Ai->GetActorLocation().Y, 0);
	
	FVector DirectionNorm = Direction.GetSafeNormal2D();
		
	FVector DesiredVelocity = DirectionNorm*Ai->GetMaxSpeed();
	
	MovementComponent->AddInputVector(DesiredVelocity);
	
	FRotator DesiredRotation = DirectionNorm.Rotation();
	
	DesiredRotation.Pitch = 0;
	
	Ai->SetActorRotation(DesiredRotation);
	
	 // when at destination change state.
	 if (ThresholdDestination(Ai) || Ai->AIJob != EJob::Medic)
	 {
	 	Ai->ChangeAiState(new SearchingState);
	 }
	else
	{
		Ai->ChangeAiState(new HealingState);
	}
}

FVector ReturnToBattle::FleeDestination(AAICharacter* AI, const FVector Location)
{
	FVector OriginalDestination = Location - FVector(AI->GetActorLocation().X,AI->GetActorLocation().Y, 0);
	OriginalDestination.Z = 0;
	
	if (OriginalDestination.Size() > MaxDistance)
	{
		OriginalDestination = OriginalDestination.GetSafeNormal2D()*MaxDistance;
	}
	else
	{
		OriginalDestination = OriginalDestination.GetSafeNormal2D()*MinDistance;

	}
	
	FVector InvertedDirection = -OriginalDestination;

	FVector FleeDestination = FVector(AI->GetActorLocation().X,AI->GetActorLocation().Y,0) + InvertedDirection;
	FleeDestination.Z = 0;
	
	return FleeDestination;
}

bool ReturnToBattle::ThresholdDestination(AAICharacter* ai)
{
	// Should have used .Size() and compared the length to my desired approximation.
	float Approximation = 20.0f;

	float X =-(Destination->X - ai->GetActorLocation().X);
	//if (X<0){X*=-1;}
	X = (X < 0) ? (-1 * X) : (X);
	
	float Y = -(Destination->Y - ai->GetActorLocation().Y);
	//if (Y<0) {Y*=-1;}
	Y = (Y < 0) ? (-1*Y) : (Y);

	// UE_LOG(LogTemp,Warning, TEXT("%f, %f"),X,Y);
	
	if (X < Approximation && Y < Approximation)
	{
		return true;
	}
	
	return false;
}