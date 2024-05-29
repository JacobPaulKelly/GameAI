#include "AI/StateMachine/RoamState.h"
#include "AI/StateMachine/FleeingState.h"
#include "AI/StateMachine/IdleState.h"
#include "GameFramework/PawnMovementComponent.h"

RoamState::RoamState()
{
	Destination = new FVector(0,0,0);
	Health = new float(0);
}

RoamState::~RoamState()
{
	delete Destination;
	Destination = nullptr;
	delete Health;
	Health = nullptr;
}

void RoamState::Setup(AAICharacter* Ai)
{
	*Health = Ai->GetCurrentHealth(); //for damage detection
	*Destination = PickNewDestination(Ai->GetActorLocation());
	MovementComponent = Ai->GetMovementComponent();
}

void RoamState::Execute(AAICharacter* Ai, float DeltaTime)
{
	//if damaged then change state immediately
	if (*Health > Ai->GetCurrentHealth())
	{
		Ai->ChangeAiState(new FleeingState);
		return;
	}
	
	const float LengthDistance = 250.0f;
	
	//move the AI
	FVector Direction = *Destination - FVector(Ai->GetActorLocation().X,Ai->GetActorLocation().Y, 0);
	float  DirectionLength = Direction.Size(); //turns direction into a length vector.
	
	//UE_LOG(LogTemp,Warning,TEXT("%f"), DirectionLength);
	
	float SlowingForce = 1.0f;
	
	if (DirectionLength < LengthDistance)
	{
		SlowingForce = DirectionLength/LengthDistance;
		if (SlowingForce<0.1){SlowingForce=0.1;}// Capping speed reduction.
	}
	
	//needs to be 2D due to randomly generated map so Z axis doesnt matter.
	FVector DirectionNorm = Direction.GetSafeNormal2D();
	
	FVector DesiredVelocity = DirectionNorm*Ai->GetMaxSpeed()*SlowingForce;
	//FVector DesiredVelocity = DirectionNorm*SlowingForce;
	//FVector DesiredVelocity = DirectionNorm*speed/DirectionLength;
	
	FVector SteeringVelocity =  DesiredVelocity - Ai->GetVelocity();
	//FVector SteeringVelocity = FMath::Lerp(Ai->GetVelocity(), DesiredVelocity, 0.1);

	//SteeringVelocity/=Ai->GetMass();
	
	MovementComponent->AddInputVector(SteeringVelocity);
	
	FRotator Rotation = DesiredVelocity.Rotation();
	
	Rotation.Pitch = 0;
	
	Ai->SetActorRotation(Rotation);
	
	//FRotator NewRotation = FMath::RInterpTo(Ai->GetActorRotation(), DesiredRotation, DeltaTime, 2);
	
	//Ai->SetActorRotation(NewRotation);
	//Creating a rotation matrix around the x axis to make character face towards desired lacation
	//next I need to make this rotate slowly instead of instantly for steering behaviours.
	//FRotator ForwardsDirection = FRotationMatrix::MakeFromX(SteeringVelocity).Rotator();
	
	//Ai->SetActorLocation(Ai->GetActorLocation()+((DirectionNorm*Ai->GetMaxSpeed())*DeltaTime));
	//Ai->SetActorLocation(Ai->GetActorLocation()+Ai->GetActorForwardVector()*Ai->GetMaxSpeed()*DeltaTime);
	
	//when at destination change state.
	if (ThresholdDestination(Ai))
	{
		Ai->ChangeAiState(new IdleState);
	}
}

FVector RoamState::PickNewDestination(const FVector Location)
{
	FVector NewDestination;
	
	float Radius = 2500;
	
	NewDestination = FVector(FMath::RandRange(Location.X-Radius,Location.X+Radius),FMath::RandRange(Location.Y-Radius,Location.Y+Radius), 0);
	
	return NewDestination;
}

bool RoamState::ThresholdDestination(AAICharacter* ai)
{
	float Approximation = 20.0f;
	
	float X = ai->GetActorLocation().X - Destination->X;
	//if (X<0){X*=-1;}
	X = (X < 0) ? (-1 * X) : (X);
	
	float Y = ai->GetActorLocation().Y - Destination->Y;
	//if (Y<0) {Y*=-1;}
	Y = (Y < 0) ? (-1*Y) : (Y);
	
	if (X < Approximation && Y < Approximation)
	{
		return true;
	}
	
	return false;
}