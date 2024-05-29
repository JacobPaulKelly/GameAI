#include "AI/StateMachine/FleeingState.h"
#include "AI/StateMachine/IdleState.h"
#include "GameFramework/PawnMovementComponent.h"

FleeingState::FleeingState()
{
	Destination = new FVector(0,0,0);
	Health = new float(0);
}

FleeingState::~FleeingState()
{
	delete Destination;
	Destination = nullptr;
	delete Health;
	Health = nullptr;
}

void FleeingState::Setup(AAICharacter* Ai)
{
	*Health = Ai->GetCurrentHealth(); //for damage detection
	*Destination = FleeDestination(Ai,Ai->DamageLocation);
	MovementComponent = Ai->GetMovementComponent();
	Ai->DamageLocation = FVector::Zero();
}

void FleeingState::Execute(AAICharacter* Ai, float DeltaTime)
{
	FVector Direction =  *Destination - FVector(Ai->GetActorLocation().X,Ai->GetActorLocation().Y, 0);
	
	FVector DirectionNorm = Direction.GetSafeNormal2D();
		
	FVector DesiredVelocity = DirectionNorm*Ai->GetMaxSpeed();
	
	MovementComponent->AddInputVector(DesiredVelocity);
	
	FRotator DesiredRotation = DirectionNorm.Rotation();
	
	DesiredRotation.Pitch = 0;
	
	Ai->SetActorRotation(DesiredRotation);

	//choose new location if damaged again.
	if (*Health > Ai->GetCurrentHealth())
	{
		Ai->ChangeAiState(new FleeingState);
		return;
	}
	
	 // when at destination change state.
	 if (ThresholdDestination(Ai))
	 {
	 	Ai->ChangeAiState(new IdleState);
	 }
}

FVector FleeingState::FleeDestination(AAICharacter* AI, const FVector Location)
{
	FVector OriginalDestination = Location - FVector(AI->GetActorLocation().X,AI->GetActorLocation().Y, 0);
	OriginalDestination.Z = 0;

	if (OriginalDestination.Size() < MinDistance)
	{
		OriginalDestination = OriginalDestination.GetSafeNormal2D()*MinDistance;
	}
	if (OriginalDestination.Size() > MaxDistance)
	{
		OriginalDestination = OriginalDestination.GetSafeNormal2D()*MaxDistance;
	}
	
	FVector InvertedDirection = -OriginalDestination;

	FVector FleeDestination = FVector(AI->GetActorLocation().X,AI->GetActorLocation().Y,0) + InvertedDirection;
	FleeDestination.Z = 0;
	
	return FleeDestination;
}

bool FleeingState::ThresholdDestination(AAICharacter* ai)
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