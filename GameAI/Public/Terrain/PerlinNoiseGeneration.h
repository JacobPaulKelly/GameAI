
#pragma once

#include "CoreMinimal.h"


class GAMEAI_API PerlinNoiseGeneration
{
public:
	PerlinNoiseGeneration();
	~PerlinNoiseGeneration();

	void InitialiseClass();

	float PerlinNoise(const FVector2D& Location);

	static float PerlinNoise(const float& LocationX, const float& LocationY);

	//UE maths for SmoothCurve this is also identical to Ken Perlin's smoothing equation
	static float SmoothCurve(float X);

	//My math for smooth curve
	static float CustomSmoothCurve(float X);

	//My chosen smoothing algorithm in an attempt to reduce amount of calculations needed
	static float SimpleSmoothCurve(float X);

	static float OctavedPerlinNoise(const float& X, const float& Y, const int32 Octaves);
	static float OctavedPerlinNoise(const float& X, const float& Y, const int32 Octaves, float& Persistence);

	inline static bool CheckPermTable = true;
	
private:
	static FVector2D GetConstantVector(int32 Value);
	
	// Need a permutation table, values from 0-255, 256 times twice.

	//inline static int32* Permutations = new int32[512];
	
	inline static const int32 Permutations[512] =
	{
		253, 6, 98, 154, 252, 231, 94, 0, 245, 187, 239, 113, 128, 194, 29, 93, 27, 244, 3, 148, 23, 153, 228, 193, 96, 147, 181, 75, 8, 139, 216, 34, 156,
		87, 28, 155, 167, 89, 18, 45, 120, 112, 242, 91, 195, 82, 66, 198, 176, 68, 134, 243, 14, 175, 30, 78, 247, 63, 209, 77, 240, 173, 16, 11, 169, 146,
		64, 230, 17, 172, 56, 110, 234, 170, 214, 58, 188, 254, 227, 165, 80, 60, 15, 232, 255, 73, 107, 168, 71, 105, 135, 12, 143, 197, 20, 85, 130, 136,
		151, 235, 186, 86, 5, 95, 251, 160, 62, 65, 217, 39, 9, 225, 119, 222, 131, 241, 184, 159, 133, 125, 144, 81, 166, 114, 182, 13, 50, 221, 249, 1,
		196, 24, 177, 215, 59, 61, 150, 174, 51, 57, 126, 236, 90, 142, 250, 163, 124, 116, 106, 127, 99, 189, 226, 183, 132, 204, 55, 4, 200, 199, 248, 246,
		109, 67, 92, 103, 123, 202, 47, 121, 54, 88, 192, 46, 36, 185, 48, 31, 157, 49, 26, 97, 115, 218, 70, 100, 220, 203, 180, 111, 212, 213, 210, 32, 237,
		158, 21, 190, 53, 43, 38, 206, 122, 2, 84, 25, 140, 52, 219, 69, 74, 201, 7, 118, 108, 19, 79, 233, 224, 76, 10, 179, 162, 164, 223, 35, 44, 211, 229,
		171, 42, 72, 238, 149, 207, 145, 137, 104, 40, 117, 22, 129, 83, 37, 102, 33, 101, 161, 191, 152, 205, 178, 141, 41, 208, 138,
	
		253, 6, 98, 154, 252, 231, 94, 0, 245, 187, 239, 113, 128, 194, 29, 93, 27, 244, 3, 148, 23, 153, 228, 193, 96, 147, 181, 75, 8, 139, 216, 34, 156,
		87, 28, 155, 167, 89, 18, 45, 120, 112, 242, 91, 195, 82, 66, 198, 176, 68, 134, 243, 14, 175, 30, 78, 247, 63, 209, 77, 240, 173, 16, 11, 169, 146,
		64, 230, 17, 172, 56, 110, 234, 170, 214, 58, 188, 254, 227, 165, 80, 60, 15, 232, 255, 73, 107, 168, 71, 105, 135, 12, 143, 197, 20, 85, 130, 136,
		151, 235, 186, 86, 5, 95, 251, 160, 62, 65, 217, 39, 9, 225, 119, 222, 131, 241, 184, 159, 133, 125, 144, 81, 166, 114, 182, 13, 50, 221, 249, 1,
		196, 24, 177, 215, 59, 61, 150, 174, 51, 57, 126, 236, 90, 142, 250, 163, 124, 116, 106, 127, 99, 189, 226, 183, 132, 204, 55, 4, 200, 199, 248, 246,
		109, 67, 92, 103, 123, 202, 47, 121, 54, 88, 192, 46, 36, 185, 48, 31, 157, 49, 26, 97, 115, 218, 70, 100, 220, 203, 180, 111, 212, 213, 210, 32, 237,
		 158, 21, 190, 53, 43, 38, 206, 122, 2, 84, 25, 140, 52, 219, 69, 74, 201, 7, 118, 108, 19, 79, 233, 224, 76, 10, 179, 162, 164, 223, 35, 44, 211, 229,
		171, 42, 72, 238, 149, 207, 145, 137, 104, 40, 117, 22, 129, 83, 37, 102, 33, 101, 161, 191, 152, 205, 178, 141, 41, 208, 138
	 };
};