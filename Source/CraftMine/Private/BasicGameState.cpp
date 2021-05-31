// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameState.h"

int32 ABasicGameState::GetScore()
{
	return Score;
}

void ABasicGameState::IncrementScore()
{
	Score += IncrementAmount;
}

void ABasicGameState::DecrementScore()
{
	Score -= DecrementAmount;
}
