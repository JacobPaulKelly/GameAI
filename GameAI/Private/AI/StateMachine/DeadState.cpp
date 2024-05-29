#include "AI/StateMachine/DeadState.h"

DeadState::DeadState()
{
	DestructionTimer = 0.0f;
}

DeadState::~DeadState()
{
}

void DeadState::Setup(AAICharacter* Ai)
{
	// Ai->GetComponentByClass<USkeletalMeshComponent>()->SetCollisionResponseToAllChannels(ECR_Block);
	// Ai->GetComponentByClass<USkeletalMeshComponent>()->UpdateCollisionProfile();
	Ai->GetComponentByClass<USkeletalMeshComponent>()->SetSimulatePhysics(true);

	Ai->SetIsDead(true); //making sure bIsDead = true
}

void DeadState::Execute(AAICharacter* Ai, float DeltaTime)
{
	DestructionTimer += DeltaTime;

	Ai->SetIsDead(true);
	
	if(DestructionTimer > 5.0f)
	{
		 // for (int32 i = 0 ; i < Ai->TeamMates.size(); ++i)
		 // {
		 // 	if(Ai->TeamMates[i])
		 // 	{
		 // 		Ai->TeamMates[i]->RemoveFromTeamList(Ai);
		 // 	}
		 // }
		
		Ai->Destroy();
	}
}
