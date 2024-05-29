
#include "AI/AICharacter.h"

#include "AI/StateMachine/DeadState.h"
#include "AI/StateMachine/IdleState.h"
#include "AI/StateMachine/ReturnToBattle.h"
#include "AI/StateMachine/SearchingState.h"
#include "AI/StateMachine/HealingState.h"
#include "GameAI/GameAICharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsDead = false;
	
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Sphere"));
	DetectionSphere->SetupAttachment(GetRootComponent());
	DetectionSphere->SetSphereRadius(1500.0f);
	
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnBeginDetectionSphereOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AAICharacter::OnEndDetectionSphereOverlap);

	DetectionSphere->SetGenerateOverlapEvents(true);
	
	DamageLocation = FVector::Zero();
	Mass = new float (100);
	MaxSpeed = new float (100);
	MaxForce = new float (0.1);
	MaxTurnRate = new float (5);
	Health = MaxHealth;
	WalkSpeed = 20;
}

AAICharacter::~AAICharacter()
{
	delete AIState;
	AIState = nullptr;
	delete Mass;
	Mass = nullptr;
	delete MaxSpeed;
	MaxSpeed = nullptr;
	delete MaxForce;
	MaxForce = nullptr;
	delete MaxTurnRate;
	MaxTurnRate = nullptr;
}

void AAICharacter::ChangeAiState(AIStateBase<AAICharacter>* NewState)
{
	delete AIState;
	AIState = NewState;
	AIState->Setup(this);
}

ETeam AAICharacter::GetTeam()
{
	return AITeam;
}

float AAICharacter::GetMass()
{
	return *Mass;
}

float AAICharacter::GetMaxSpeed()
{
	return *MaxSpeed;
}

float AAICharacter::GetMaxForce()
{
	return *MaxForce;
}

float AAICharacter::GetMaxTurnRate()
{
	return *MaxTurnRate;
}

float AAICharacter::GetCurrentHealth()
{
	return Health;
}

void AAICharacter::UpdateTeamList()
{
	TArray<AActor*> AllAIActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AAICharacter::StaticClass(), AllAIActors);

	int32 i = 0;

	if (!TeamMates.empty())
	{
		
		for(auto It = TeamMates.rend(); It != TeamMates.rbegin();)
		{
			It = decltype(TeamMates)::reverse_iterator(TeamMates.erase(It.base()));
		}
		TeamMates.shrink_to_fit();
	}
	
	
	for (auto It = AllAIActors.CreateConstIterator(); It; ++It)
	{
		
		AAICharacter* Actor = Cast<AAICharacter>(*It);

		if (Actor->AITeam == this->AITeam && Actor != this)
		{
			//TeamMates[i] = Actor;
			TeamMates.push_back(Actor);
			++i;
		}
	}
	
	// for (int32 j = 0; j<i ;++j)
	// {
	// 	const AActor* Team = TeamMates[j];
	//
	// 	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Team->GetName());
	// }
}

void AAICharacter::RemoveFromTeamList(AAICharacter* TeamMate)
{
	//caused engine to freeze
	// for(auto It = TeamMates.begin(); It != TeamMates.end();)
	// {
	// 	if (*It == TeamMate)
	// 	{
	// 		It = TeamMates.erase(It);
	// 		TeamMates.shrink_to_fit();
	// 		
	// 		return;
	// 	}
	// }
	
	int32 i = 0;
	for(auto It = TeamMates.begin(); It < TeamMates.end();)
	{
		if(i >= TeamMates.size())
		{
			return;
		}
		if(!TeamMates[i]) // defensive coding as this was causing a lot of issues
		{
			return;
		}
		if (TeamMates[i] == TeamMate)
		{
			TeamMates.erase(It);
			TeamMates.shrink_to_fit();
			
			return;
		}
		++i;
	}
}

void AAICharacter::AddToEnemyList(AAICharacter* Enemy)
{
	Enemies.push_back(Enemy);
}

void AAICharacter::RemoveFromEnemyList(AAICharacter* Enemy)
{
	// for (int32 i = 0; i < Enemies.size(); ++i)
	
	for(auto It = Enemies.begin(); It != Enemies.end(); ++It)
	{
		if (*It == Enemy)
		{
			It = Enemies.erase(It);
			Enemies.shrink_to_fit();
			
			return;
		}
	}
}

void AAICharacter::DamageHealth(AActor* actor, float Damage, FVector Location)
{
	Health -= Damage;

	DamageLocation = Location;
	if (bIsDead)
	{
		return;
	}
	
	if (Health <=0)
	{
		bIsDead = true;
		
		ChangeAiState(new DeadState);
		return;
	}
	
	if (!Cast<AAICharacter>(actor) && AITeam != ETeam::None)
	{
		ChangeAiState(new ReturnToBattle);
	}
}

void AAICharacter::OnBeginDetectionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAICharacter>(OtherActor))
	{
		//UE_LOG(LogTemp, Warning, TEXT("An AI character has been detected"));
		
		AAICharacter* Enemy;
		
		Enemy = Cast<AAICharacter>(OtherActor);
		
		if (Enemy->AITeam != this->AITeam)
		{
			//UE_LOG(LogTemp, Warning, TEXT("An enemy has been detected"));

			bool bDontAdd = false;

			for (int32 i = 0; i < Enemies.size(); ++i)
			{
				if (Enemy == Enemies[i])
				{
					bDontAdd = true;
				}
			}
			if (!bDontAdd)
			{
				AddToEnemyList(Enemy);
			}
		}
	}
}

void AAICharacter::OnEndDetectionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (Cast<AAICharacter>(OtherActor))
	{
		AAICharacter* Enemy;
		
		Enemy = Cast<AAICharacter>(OtherActor);
		if (Enemy->AITeam != this->AITeam)
		{
			RemoveFromEnemyList(Enemy);
		}
	}
}

void AAICharacter::SetIsDead(bool IsDead)
{
	bIsDead = IsDead;
}

bool AAICharacter::GetIsDead()
{
	return bIsDead;
}

void AAICharacter::ReturnToBattleField()
{
	this->ChangeAiState(new ReturnToBattle);
}

void AAICharacter::HealDamage(float HealAmmount)
{
	Health += HealAmmount;
	
	if (Health > 100)
	{
		Health = 100;
	}
	//UE_LOG(LogTemp, Error, TEXT("Health = %f"), Health);
}

float AAICharacter::GetWalkSpeed()
{
	return WalkSpeed;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	switch (AITeam)
	{
	case 0: // No Team
		AIJob = EJob::Jobless; // Ensuring those without a team dont do team behaviours
		SetupJob();
		break;
		
	case 1: // Red Team
		if (AIJob == EJob::Jobless)	{AIJob = EJob::Soldier;} // Ensuring that if a team was selected but a job wasn't then a default job is applied.
		SetTeamColourRed();
		SetupJob();
		break;
		
	case 2:// Blue Team
		if (AIJob == EJob::Jobless) {AIJob = EJob::Soldier;} // Ensuring that if a team was selected but a job wasn't then a default job is applied.
		SetTeamColourBlue();
		SetupJob();
		break;
		
	default:
		break;
	}

	UpdateTeamList();
}

void AAICharacter::SetTeamColourRed()
{
	USkeletalMeshComponent* skin = this->GetComponentByClass<USkeletalMeshComponent>();
	
	skin->SetMaterial(0,RedMaterial);
}

void AAICharacter::SetTeamColourBlue()
{
	USkeletalMeshComponent* skin = this->GetComponentByClass<USkeletalMeshComponent>();
	
	skin->SetMaterial(0,BlueMaterial);
}

void AAICharacter::SetupJob()
{
	switch (AIJob)
	{
	case 0: // Jobless
		AIState = new IdleState;
		AIState->Setup(this);
		break;
		
	case 1: // Soldier
		AIState = new SearchingState;
		AIState->Setup(this);
		break;
		
	case 2: // Medic (not implemented atm)
		AIState = new HealingState;
		AIState->Setup(this);
		break;
		
	default:
		UE_LOG(LogTemp, Error, TEXT("ENUM: AI Job. Failed"));
			break;
	}
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AIState->Execute(this,DeltaTime);
}