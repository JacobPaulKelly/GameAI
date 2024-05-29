
#include "Terrain/RandomTerrainGeneration.h"
#include "Terrain/PerlinNoiseGeneration.h"
#include "RNGGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARandomTerrainGeneration::ARandomTerrainGeneration()
{
	Terrain = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Terrain"));
	Terrain->SetupAttachment(GetRootComponent());

	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));
	Water->SetupAttachment(GetRootComponent());
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UVScale = 1;
}

ARandomTerrainGeneration::~ARandomTerrainGeneration()
{
}

// Called when the game starts or when spawned
void ARandomTerrainGeneration::BeginPlay()
{
	Super::BeginPlay();

	// I should be using an assert here but I didnt want the engine to crash so instead created
	// a check that outputs to the log if something is wrong but allows the game to proceed regarless.
	if (Cast<URNGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GenSeed = Cast<URNGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetSeed();

		WaterHeight = Cast<URNGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetWaterHeight();
		
		if (GenSeed<=0)
		{
			GenSeed = 0;
			WaterHeight = 0;
		}
		
		UE_LOG(LogTemp,Error,TEXT("Seed = %i"), GenSeed);
		UE_LOG(LogTemp,Error,TEXT("WaterHeight = %f"), WaterHeight);
		
		FillPermutationTable(GenSeed);
	}
	else
	{
		GenSeed = 0;
		WaterHeight = 0.0f;
		
		FillPermutationTable(GenSeed);
		
		UE_LOG(LogTemp,Error,TEXT("Failed to get GameState"));
	}
	
	CreateVertices();
	CreateTriangles();
	
	Terrain->CreateMeshSection(0,Vertices, Triangles, Normals, UV0, VertexColours, Tangents, true);
	Terrain->SetMaterial(0, TerrainMaterial);

	// Terrain->SetCollisionResponseToAllChannels(ECR_Block);

	FRandomStream RandomStream;
	RandomStream.Initialize(GenSeed);
	
	Water->SetRelativeLocation(FVector(0,0,WaterHeight));
	Water->SetRelativeScale3D(FVector(500,500,1));
	Water->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARandomTerrainGeneration::FillPermutationTable(int32 Seed)
{
	FRandomStream RandomStream;
	RandomStream.Initialize(Seed);
	
	int32* Numbers = new int32[256];

	// fisher-yates shuffle algorithm

	for (int32 i = 0; i<256;++i)
	{
		Numbers[i] = i;
	}

	for (int32 i = 255; i >=0; --i)
	{
		if (i!=0)
		{
		int32 J = RandomStream.RandRange(0, i+1);
		int32 temp = Numbers[i];
		Numbers[i] = Numbers[J];
		Numbers[J] = temp;
		}
		Permutations[i] = Numbers[i];
		Permutations[i+256] = Numbers[i];
	}

	// for (int32 i = 0; i<512;++i)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Permuataion %i = %i"), i, Permutations[i]);
	// }

	delete[] Numbers;
	Numbers = nullptr;
	
	// Old method caused hanging in unreal engine.
	
	// bool isDuplicate = false;
	//
	// int32 permutation;
	//
	// for (int32 i = 0; i < 256; ++i)
	// {
	// 	permutation = RandomStream.RandRange(0,255);
	//
	// 	for (int32 j = 0; j <= i; ++j)
	// 	{
	// 		if (permutation == Permutations[j])
	// 		{
	// 			isDuplicate = true;
	// 			--i;
	// 		}
	// 	}
	// 	
	// 	if (isDuplicate)
	// 	{
	// 		isDuplicate = false;
	// 		continue;
	// 	}
	// 	
	// 	Permutations[i] = permutation;
	// 	Permutations[i+256] = permutation;
	// }
}

int32* ARandomTerrainGeneration::GetPermutationTable()
{
	return Permutations;
}

void ARandomTerrainGeneration::CreateVertices()
{
	
	for (int32 x = 0; x <= ChunkSize; ++x)
	{
		for (int32 y = 0; y <= ChunkSize; ++y)
		{
			// const float z = PerlinNoiseGeneration::OctavedPerlinNoise(x*NoiseScale+0.01f, y*NoiseScale+0.01, Octaves)* ZMultiplier; //turns x and y location into a unit location.
			
			const float z = PerlinNoiseGeneration::OctavedPerlinNoise(x*NoiseScale+0.01f, y*NoiseScale+0.01, Octaves, Persistence)* ZMultiplier; //turns x and y location into a unit location.
			// const float n1z = PerlinNoiseGeneration::OctavedPerlinNoise(x*NoiseScale+0.01f, y*NoiseScale+0.01, Octaves, Persistence)* ZMultiplier; //turns x and y location into a unit location.
			// const float n2z = PerlinNoiseGeneration::OctavedPerlinNoise(x*NoiseScale+0.01f, y*NoiseScale+0.01, Octaves, Persistence)* ZMultiplier; //turns x and y location into a unit location.
			// const float n3z = PerlinNoiseGeneration::OctavedPerlinNoise(x*NoiseScale+0.01f, y*NoiseScale+0.01, 6, Persistence)* ZMultiplier; //turns x and y location into a unit location.

			// const float z = PerlinNoiseGeneration::PerlinNoise(x * NoiseScale+0.01,y * NoiseScale+0.01)*ZMultiplier; //turns x and y location into a unit location.
			
			//const float z = PerlinNoiseGeneration::OctavedPerlinNoise(x*NoiseScale+0.01f, y*NoiseScale+0.01, Octaves, Persistence)* ZMultiplier; //turns x and y location into a unit location.

			// My attempt to create unit squares causes issues and doesnt generate anything properly.
			//const float z = PerlinNoiseGeneration::OctavedPerlinNoise(x/SizeX, y/SizeY)*ZMultiplier;
			//const float z = PerlinNoiseGeneration::PerlinNoise(x/SizeX,y/SizeY)*ZMultiplier;

			//unreal engines to compare with custom.
			//const float z = FMath::PerlinNoise2D(FVector2D(x * NoiseScale+0.01,y * NoiseScale+0.01))*ZMultiplier;
			
			// UE_LOG(LogTemp,Warning, TEXT("%f"),z)
						
			Vertices.Add(FVector(x*Scale-(ChunkSize*Scale*0.5), y*Scale-(ChunkSize*Scale*0.5), z));
			
			UV0.Add(FVector2D(x*UVScale,y*UVScale));
		}
	}
}

void ARandomTerrainGeneration::CreateTriangles()
{
	//loop over each vertex and create a triangle

	int32 Vertex = 0;
	
	for (int32 j = 0; j < ChunkSize; ++j)
	{
		for	(int32 i = 0; i < ChunkSize; ++i)
		{
			Triangles.Add(Vertex); // Bottom left vertex
			Triangles.Add(Vertex+1); // Bottom right vertex
			Triangles.Add(Vertex+ChunkSize+1); // top left vertex
			Triangles.Add(Vertex+1); // Bottom right vertex
			Triangles.Add(Vertex+ChunkSize+2); // Top right vertex
			Triangles.Add(Vertex+ChunkSize+1); // top left vertex

			++Vertex;
		}
		++Vertex;
	}
}