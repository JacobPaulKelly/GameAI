#pragma once
#include "AIStateBase.h"
#include "AI/AICharacter.h"

class RoamState: public AIStateBase<AAICharacter>
{
public:
	RoamState();
	~RoamState() override;

	void Setup(AAICharacter* Ai) override;
	void Execute(AAICharacter* Ai, float DeltaTime) override;

private:
	FVector* Destination;
	float* Health;

	FVector PickNewDestination(const FVector Location);

	bool ThresholdDestination(AAICharacter* ai);

	UPawnMovementComponent* MovementComponent;
};
