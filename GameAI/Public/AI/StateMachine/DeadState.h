#pragma once
#include "AIStateBase.h"
#include "AI/AICharacter.h"


class DeadState:public  AIStateBase<AAICharacter>
{
public:
	DeadState();
	~DeadState() override;

	void Setup(AAICharacter* Ai) override;
	void Execute(AAICharacter* Ai, float DeltaTime) override;

private:
	float DestructionTimer;
};