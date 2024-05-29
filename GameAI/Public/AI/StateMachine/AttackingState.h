#pragma once

#include "AIStateBase.h"
#include "AI/AICharacter.h"

class AttackingState:public AIStateBase<AAICharacter>
{
public:
	AttackingState();
	~AttackingState() override;

	void Setup(AAICharacter* Ai) override;
	void Execute(AAICharacter* Ai, float DeltaTime) override;
	
	FVector AimDirectionWithAccuracy(FVector PreviousDirection);

	void LineTrace(AAICharacter* AI);
	
private:
	float* FireRate;//RPS
	float* Cooldown;
	const float AccuracyOffset = 0.07f;	
	FVector* Accuracy;
	float* Damage;
	FVector* AimDirection;
	float* TracelineLength;

	UPawnMovementComponent* MovementComponent;
};