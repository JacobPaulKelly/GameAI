// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RNGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEAI_API URNGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	int32 GetSeed();
	UFUNCTION(BlueprintCallable)
	void SetSeed(int32 seed);

	UFUNCTION(BlueprintCallable)
	float GetWaterHeight();
	UFUNCTION(BlueprintCallable)
	void SetWaterHeight(float waterHeight);

private:
	int32 Seed;

	float WaterHeight;
};
