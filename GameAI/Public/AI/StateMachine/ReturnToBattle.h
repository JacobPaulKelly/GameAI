#pragma once

#include "AIStateBase.h"
#include "AI/AICharacter.h"

class ReturnToBattle : public AIStateBase<AAICharacter>
{
public:
	ReturnToBattle();
	~ReturnToBattle() override;

	void Setup(AAICharacter* Ai) override;
	void Execute(AAICharacter* Ai, float DeltaTime) override;

private:
	FVector* Destination;

	FVector FleeDestination(AAICharacter* AI, const FVector Location);
	
	bool ThresholdDestination(AAICharacter* ai);

	UPawnMovementComponent* MovementComponent;

	const float MaxFleeDistance = 1000;

	const float MinDistance = 1500;
	const float MaxDistance = 3000;
};
