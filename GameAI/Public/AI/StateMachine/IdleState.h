#pragma once
#include "AIStateBase.h"
#include "AI/AICharacter.h"

class IdleState:public AIStateBase<AAICharacter>
{
public:
	IdleState();
	~IdleState() override;

	void Setup(AAICharacter* Ai) override;
	void Execute(AAICharacter* Ai, float DeltaTime) override;

private:
	float* Health;
	
	float* WaitTime;
};