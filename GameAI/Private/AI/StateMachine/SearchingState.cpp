#include "AI/StateMachine/SearchingState.h"
#include "AI/StateMachine/AttackingState.h"
#include "GameFramework/PawnMovementComponent.h"

SearchingState::SearchingState()
{
	UpdateTimer = new float(0.0f);
	Radius = new float(5.0f);
	WanderDistance = new float(500.0f);
	WanderJitter = new float (35.0f); //amount of degrees to move
	WanderAngle = new float(0.0f);

	DirectionTimer = 0;	
}

SearchingState::~SearchingState()
{
	delete UpdateTimer;
	UpdateTimer = nullptr;
	delete Radius;
	Radius = nullptr;
	delete WanderDistance;
	WanderDistance = nullptr;
	delete WanderJitter;
	WanderJitter = nullptr;
	delete WanderAngle;
	WanderAngle = nullptr;
}

void SearchingState::Setup(AAICharacter* Ai)
{
	UpdateAngle();
}

void SearchingState::Execute(AAICharacter* Ai, float DeltaTime)
{
	if (!Ai->Enemies.empty())
	{
		for (int32 i = 0; i < Ai->Enemies.size(); ++i)
		{
			if (!Ai->Enemies[i]->GetIsDead())
			{
				Ai->ChangeAiState(new AttackingState);
			
				return; // No need to check further, we already know not all enemies are alive
			}
		}
	}


	FVector DesiredLocation = Calculate(Ai);

	DesiredLocation = DesiredLocation.GetSafeNormal() * Ai->GetMaxSpeed();

	Ai->GetMovementComponent()->AddInputVector(DesiredLocation);

	Ai->SetActorRotation(DesiredLocation.Rotation());
	
	if (*UpdateTimer > 0.1)
	{
		UpdateAngle();
	}
	
	*UpdateTimer += DeltaTime;

	
}

FVector SearchingState::Calculate(AAICharacter* AI)
{

	FVector WanderLocation = AI->GetActorForwardVector() * *WanderDistance; //wanderpoint

	//float Theta = 3.14159;
	//float Theta = 3.14159/2;
	//float Theta = 0;

	float X = *Radius * FMath::Cos(*WanderAngle);
	float Y = *Radius * FMath::Sin(*WanderAngle);

	//float X = WanderLocation.X + *Radius * FMath::Cos(FMath::DegreesToRadians(*WanderAngle));
	//float Y = WanderLocation.Y + *Radius * FMath::Sin(FMath::DegreesToRadians(*WanderAngle));
	
	FVector DesiredLocation = FVector(WanderLocation.X + X, WanderLocation.Y + Y, 0);
	
	return DesiredLocation;
}

void SearchingState::UpdateAngle()
{
	*WanderAngle = FMath::RandRange(-*WanderJitter, *WanderJitter);

	*UpdateTimer = 0;
}
