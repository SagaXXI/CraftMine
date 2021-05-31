// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BasicGameState.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTMINE_API ABasicGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	int32 GetScore();

	void IncrementScore();

	void DecrementScore();

	UPROPERTY(EditAnywhere, Category = "Score")
	int32 MaxScore = 100;

private:

	int32 Score = 0;

	UPROPERTY(EditAnywhere, Category = "Score")
	int32 IncrementAmount = 7;

	UPROPERTY(EditAnywhere, Category = "Score")
	int32 DecrementAmount = 1;
	
	
};
