// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain/WorldGenerator.h"
#include "RNGGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWorldGenerator::AWorldGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();
	SeedToUse = Cast<URNGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetSeed();
	
	//SetTerrainSize(Chunk);

	GetWorld()->SpawnActor<ARandomTerrainGeneration>(Chunk,FVector(0,0,0), FRotator(0,0,0));

}

void AWorldGenerator::SetTerrainSize(TSubclassOf<ARandomTerrainGeneration> piece)
{
	ARandomTerrainGeneration* Newpiece = Cast<ARandomTerrainGeneration>(piece);

	//Newpiece->Octaves=8;
}