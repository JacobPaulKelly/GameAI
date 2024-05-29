// Fill out your copyright notice in the Description page of Project Settings.


#include "RNGGameInstance.h"

int32 URNGGameInstance::GetSeed()
{
	return Seed;
}

void URNGGameInstance::SetSeed(int32 seed)
{
	Seed = seed;
}

float URNGGameInstance::GetWaterHeight()
{
	return WaterHeight;
}

void URNGGameInstance::SetWaterHeight(float waterHeight)
{
	WaterHeight = waterHeight;
}
