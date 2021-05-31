// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MineCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "BasicGameMode.generated.h"

/**
 * 
 */
class AMineCharacter;
class AMyPlayerController;
class AOre;
class ABasicGameState;

UCLASS()
class CRAFTMINE_API ABasicGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	void DestroyedActor(AOre* DestroyedActor, AActor* Destroyer);

	UFUNCTION(BlueprintImplementableEvent)	
	void GameStart();
	
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);

protected:
	virtual void BeginPlay() override;

	private:
	void HandleGameStart();

	void HandleGameOver(bool PlayerWon);

	void PlayerWon();

	void PlayerLost();

	void StartDecrementing();

	FTimerHandle Decrementing;
	
	FTimerDelegate ScoreDecrement;

	UPROPERTY(EditAnywhere, Category = "Score")
	float DecrementDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	int StartDelay = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	AMineCharacter* PlayerPawn;

	AMyPlayerController* PlayerControllerRef;

	AMineCharacter* PlayerCharacter;

	ABasicGameState* GS;
};

