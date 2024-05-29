#pragma once
#include "AI/AICharacter.h"
#include "AI/StateMachine/AIStateBase.h"

class HealingState: public AIStateBase<AAICharacter>
{
public:
	HealingState();
	~HealingState() override;

	void Setup(AAICharacter* Ai) override;
	void Execute(AAICharacter* Ai, float DeltaTime) override;

private:
	float LowestHealth;
	FVector Destination;
	FVector Direction;
	const float SlowDistanceMin = 500.0f;
	const float SlowDistanceMax = 2000.0f;
	const float AccuracyOffset = 0.01f;
	float SlowStrength;
	FVector Accuracy;

	const float FireRate = 2.0f;
	float Cooldown;

	FVector DirectionVelocity;

	const float TracelineLength = 1000;

	const float HealAmmount = 2.5f;

	void LineTrace(AAICharacter* AI);

	UPawnMovementComponent* MovementComponent;

	AAICharacter* Target;

	FVector AimDirectionWithAccuracy(FVector OriginalAim);
};
