
#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/StateMachine/AIStateBase.h"
#include "Components/SphereComponent.h"
#include "AICharacter.generated.h"

UENUM(BlueprintType)
enum class ETeam: uint8
{
	None UMETA(DisplayName = "No Team"),
	Red UMETA(DisplayName = "Red Team"),
	Blue UMETA(DisplayName = "Blue Team")
};

UENUM(BlueprintType)
enum class EJob
{
	Jobless UMETA(DisplayName = "No Job"),
	Soldier UMETA(DisplayName = "Soldier"),
	Medic UMETA(DisplayName = "Medic")
};

UCLASS()
class GAMEAI_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();
	~AAICharacter() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	AIStateBase<AAICharacter>* AIState;
	
	void ChangeAiState(AIStateBase<AAICharacter>* NewState);
	
	UPROPERTY(EditAnywhere)
	ETeam AITeam;

	UPROPERTY(EditAnywhere)
	EJob AIJob;
	
	ETeam GetTeam();
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* RedMaterial;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* BlueMaterial;
	
	FVector DamageLocation;
	
	float GetMass();
	float GetMaxSpeed();
	float GetMaxForce();
	float GetMaxTurnRate();

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	void UpdateTeamList();
	void RemoveFromTeamList(AAICharacter* TeamMate);
	
	void AddToEnemyList(AAICharacter* Enemy);
	void RemoveFromEnemyList(AAICharacter* Enemy);

	UFUNCTION(BlueprintCallable)
	void DamageHealth(AActor* actor, float Damage, FVector Location);

	UPROPERTY(EditAnywhere)
	USphereComponent* DetectionSphere;
	
	UFUNCTION()
	void OnBeginDetectionSphereOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndDetectionSphereOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
	std::vector<AAICharacter*> TeamMates;
	std::vector<AAICharacter*> Enemies;

	void SetIsDead(bool bIsDead);
	bool GetIsDead();

	void ReturnToBattleField();

	void HealDamage(float HealAmmount);

	float GetWalkSpeed();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsDead;
	
	void SetTeamColourRed();
    void SetTeamColourBlue();
	void SetupJob();
	
	float* Mass;
	float* MaxSpeed;
	float* MaxForce;
	float* MaxTurnRate;
	const float MaxHealth = 100;
	float Health;
	float WalkSpeed;

	//AAICharacter* TeamMates[50];

	//AAICharacter* Enemies[];
};