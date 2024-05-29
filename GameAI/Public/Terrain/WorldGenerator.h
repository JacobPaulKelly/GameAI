// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomTerrainGeneration.h"
#include "WorldGenerator.generated.h"

UCLASS()
class GAMEAI_API AWorldGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGenerator();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARandomTerrainGeneration> Chunk;
	
	UFUNCTION(BlueprintCallable)
	void SetTerrainSize(TSubclassOf<ARandomTerrainGeneration> piece);
	//void SetTerrainSize(AActor* piece);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1), meta = (ClampMax = 2000))
	int32 ChunkSize;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1), meta = (ClampMax = 8))
	int32 Octaves; // the amount of times to iterate over perlin noise's lacunarity and Amplitude
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 2))
	float ZMultiplier;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	float Scale;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.000001))
	float NoiseScale;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1), meta = (ClampMax = 215)) 
	float Persistence;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* TerrainMaterial;

private:
	int32 SeedToUse;

};
