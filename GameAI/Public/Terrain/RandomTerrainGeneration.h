#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "RandomTerrainGeneration.generated.h"

UCLASS()
class GAMEAI_API ARandomTerrainGeneration : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomTerrainGeneration();
	~ARandomTerrainGeneration();

	UPROPERTY(EditAnywhere, meta = (ClampMax = 2000))
	int32 ChunkSize;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	float ZMultiplier;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.00000001))
	float Scale;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.00000001))
	float NoiseScale;

	//after 215 results begin to not do what expected and less than one stops octaves from working correctly.
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1), meta = (ClampMax = 215)) 
	float Persistence;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1), meta = (ClampMax = 8))
	int32 Octaves; // the amount of times to iterate over perlin noise's lacunarity and Amplitude

	float UVScale;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* TerrainMaterial;

	UPROPERTY(EditAnywhere)
	int32 SeedToUse;
	
	void FillPermutationTable(int32 Seed);
	
	static int32* GetPermutationTable();
	
	inline static int32* Permutations = new int32[512];
	
UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Water;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

private:
	//everything needed to spawn a mesh
	//*
	float WaterHeight;
	
	UProceduralMeshComponent* Terrain;
	
	TArray<FVector> Vertices;
	TArray<FVector> Normals;

	TArray<int32> Triangles;
	
	TArray<FVector2D> UV0;
	
	TArray<FColor> VertexColours;
	
	TArray<FProcMeshTangent> Tangents;
	//*

	void CreateVertices();
	void CreateTriangles();

	int32 GenSeed = 0;
};
