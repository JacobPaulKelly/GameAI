
#include "Terrain/PerlinNoiseGeneration.h"
#include "VectorTypes.h"
#include "Terrain/RandomTerrainGeneration.h"

PerlinNoiseGeneration::PerlinNoiseGeneration()
{
	//InitialiseClass();
}

PerlinNoiseGeneration::~PerlinNoiseGeneration()
{
}

void PerlinNoiseGeneration::InitialiseClass()
{
	//Permutations = ARandomTerrainGeneration::GetPermutationTable();
}

float PerlinNoiseGeneration::PerlinNoise(const FVector2D& Location)
{
	return PerlinNoise(Location.X,Location.Y);
}

float PerlinNoiseGeneration::PerlinNoise(const float& LocationX, const float& LocationY)
{
	// Rounding down
	float FlooredX = FMath::Floor(LocationX);
	float FlooredY = FMath::Floor(LocationY);

	//converting to int and using bitwise & operator to cause wrapping around 0-255
	int32 XInterger = static_cast<int32>(FlooredX)&255;
	int32 YInterger = static_cast<int32>(FlooredY)&255;
	
	float X = LocationX - FlooredX;
	float Y = LocationY - FlooredY;

	//get my seeded random permutation table.
	const int32* permutation = ARandomTerrainGeneration::GetPermutationTable();
	//const int32* permutation = Permutations;

	FVector2D BottomLeft = FVector2D(X,Y);
	FVector2D BottomRight = FVector2D(X-1,Y);
	FVector2D TopLeft = FVector2D(X,Y-1);
	FVector2D TopRight = FVector2D(X-1,Y-1);

	const int32 BottomLeftValue = permutation[permutation[XInterger]+YInterger];
	const int32 BottomRightValue = permutation[permutation[XInterger+1]+YInterger];
	const int32 TopLeftValue = permutation[permutation[XInterger]+YInterger+1];
	const int32 TopRightValue = permutation[permutation[XInterger+1]+YInterger+1];

	float DotBottomLeft = FVector2D::DotProduct(BottomLeft,GetConstantVector(BottomLeftValue));
	float DotBottomRight = FVector2D::DotProduct(BottomRight,GetConstantVector(BottomRightValue));
	float DotTopLeft = FVector2D::DotProduct(TopLeft,GetConstantVector(TopLeftValue));
	float DotTopRight = FVector2D::DotProduct(TopRight,GetConstantVector(TopRightValue));

	 // float U = SmoothCurve(X);
	 // float V = SmoothCurve(Y);
	
	// float U = CustomSmoothCurve(X);
	// float V = CustomSmoothCurve(Y);
	
	 float U = SimpleSmoothCurve(X);
	 float V = SimpleSmoothCurve(Y);
	
	return FMath::Lerp(
		FMath::Lerp(DotBottomLeft, DotTopLeft,V),
		FMath::Lerp(DotBottomRight, DotTopRight, V),
		U);
}

float PerlinNoiseGeneration::SmoothCurve(float X)
{
	return X * X * X * (X * (X * 6.0f - 15.0f) + 10.0f);
}

//doesnt work as intended
float PerlinNoiseGeneration::CustomSmoothCurve(float X)
{
	if (X < 0.5f)
	{
		return (FMath::Pow(2,20*X-10)/2);
	}
	
	return (2-FMath::Pow(2,-20*X+10)/2);
}

float PerlinNoiseGeneration::SimpleSmoothCurve(float x)
{
	return x*x*(3-2*x);
}

float PerlinNoiseGeneration::OctavedPerlinNoise(const float& X,const float& Y, const int32 Octaves)
{
	float Total = 0;
	float MaxValue = 0; // used to normalise result.
	int32 Lacunarity = 1; // Controls frequency of octaves. *Latin for gap!
	int32 Amplitude = 1; // Controls height of Octaves.
	
	for (int32 i = 0; i < Octaves; ++i)
	{
		Total += PerlinNoise(X*Lacunarity, Y*Lacunarity)*Amplitude;

		MaxValue += Amplitude;
		
		Amplitude *= 1.1;
		Lacunarity *= 2;
	}
	
	return Total/MaxValue;
}

float PerlinNoiseGeneration::OctavedPerlinNoise(const float& X,const float& Y, const int32 Octaves, float& Persistence)
{
	float Total = 0;
	float MaxValue = 0; // used to normalise result.
	int32 Lacunarity = 1; // Controls frequency of octaves. *Latin for gap!
	int32 Amplitude = 1; // Controls height of Octaves.
	
	for (int32 i = 0; i < Octaves; ++i)
	{
		Total += PerlinNoise(X*Lacunarity, Y*Lacunarity)*Amplitude;

		MaxValue += Amplitude;

		Amplitude *= Persistence;
		Lacunarity *= 2;
	}
	
	return Total/MaxValue;
}

FVector2D PerlinNoiseGeneration::GetConstantVector(int32 Value)
{
	// Value is the value from the permutation table
	//const int32 Hash = Value & 7;
	const int32 Hash = Value & 3;

	//returns better results
	switch (Hash)
	{
	case 0:
		return FVector2D(1.0, 0);
	case 1:
		return FVector2D(1.0, 1.0);
	case 2:
		return FVector2D(0, 1.0);
	case 3:
		return FVector2D(-1.0, 1.0);
	case 4:
		return FVector2D(-1.0, 0);
	case 5:
		return FVector2D(-1.0, -1.0);
	case 6:
		return FVector2D(0, -1.0);
	case 7:
		return FVector2D(1.0, -1.0);

		
	// original method
		
	// switch (Hash)
	// {
	// case 0:
	// 	return FVector2D(1.0, 1.0);
	// case 1:
	// 	return FVector2D(-1.0, 1.0);
	// case 2:
	// 	return FVector2D(-1.0, -1.0);
	// case 3:
	// 	return FVector2D(1.0, -1.0);

		
	// this shouldn't be possible
	default:
		return FVector2D (0,0);
	}
}