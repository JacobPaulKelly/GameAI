// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template <class T>
class GAMEAI_API AIStateBase
{
public:
	AIStateBase();
	virtual ~AIStateBase();

	virtual void Execute(T* Ai, float DeltaTime) = 0; //virtual function to be overidden by implemented classes.
	virtual void Setup(T* Ai) = 0;
};

template <class T>
AIStateBase<T>::AIStateBase()
{
}

template <class T>
AIStateBase<T>::~AIStateBase()
{
}
