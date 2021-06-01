// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameState.h"

int32 ABasicGameState::GetScore()
{
	return Score;
}

void ABasicGameState::IncrementScore()
{
	Score = FMath::Clamp(Score + IncrementAmount, Score, MaxScore);
}

void ABasicGameState::DecrementScore()
{
	Score = FMath::Clamp(Score - DecrementAmount, 0, Score);
}
