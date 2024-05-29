#include "AI/StateMachine/IdleState.h"
#include "AI/StateMachine/FleeingState.h"
#include "AI/StateMachine/RoamState.h"

IdleState::IdleState()
{
	Health = new float(0);
	WaitTime = new float (FMath::RandRange(2.0f,15.0f));
}

IdleState::~IdleState()
{
	delete Health;
	Health = nullptr;
	delete WaitTime;
	WaitTime = nullptr;
}

void IdleState::Setup(AAICharacter* Ai)
{
	*Health = Ai->GetCurrentHealth(); //for damage detection
}

void IdleState::Execute(AAICharacter* Ai, float DeltaTime)
{
	//if damaged then change state immediately
	if (*Health > Ai->GetCurrentHealth())
	{
		Ai->ChangeAiState(new FleeingState);
		return;
	}
	
	// UE_LOG(LogTemp,Error, TEXT("Time Remaining = %f"),*WaitTime);
	
	*WaitTime -= DeltaTime;
	
	if (*WaitTime <= 0)
	{
		Ai->ChangeAiState(new RoamState);
	}
}