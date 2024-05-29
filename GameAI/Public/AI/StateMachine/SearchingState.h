#pragma once
#include "AIStateBase.h"
#include "AI/AICharacter.h"

class SearchingState:public AIStateBase<AAICharacter>
{
public:
	SearchingState();
	~SearchingState() override;

	void Setup(AAICharacter* Ai) override;
	void Execute(AAICharacter* Ai, float DeltaTime) override;

	const float pi = 3.14159;
	
	float* UpdateTimer;
	float* Radius;
	float* WanderDistance;
	float* WanderJitter;
	float* WanderAngle; // Degrees

	FVector Calculate(AAICharacter* AI);

	float DirectionTimer;

	void UpdateAngle();
};
