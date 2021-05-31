// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameMode.h"
#include "MyPlayerController.h"
#include "MineCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Ore.h"
#include "BasicGameState.h"

void ABasicGameMode::BeginPlay()
{
    Super::BeginPlay();
    PlayerCharacter = Cast<AMineCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    GS = GetGameState<ABasicGameState>();
    ScoreDecrement = FTimerDelegate::CreateUObject(GS, &ABasicGameState::DecrementScore);
   // HandleGameStart();
   
}

void ABasicGameMode::DestroyedActor(AOre* DestroyedActor, AActor* Destroyer)
{
    if(DestroyedActor == PlayerCharacter->GetCurrentOre() && Cast<AMineCharacter>(Destroyer) == PlayerCharacter)
    {
        if(GS)
        {
            UE_LOG(LogTemp, Warning, TEXT("Score : %i"), GS->GetScore())
            GS->IncrementScore();
            UE_LOG(LogTemp, Warning, TEXT("Score : %i"), GS->GetScore())
            if(GS->GetScore() == GS->MaxScore)
            {
              //PlayerWon();
              PlayerControllerRef->SetPlayerEnableState(false);
              //HandleGameOver(true);
            }
        }
    }
}

void ABasicGameMode::HandleGameStart()
{
    GameStart();
    if(PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnableState(false);

        FTimerHandle PlayerEnableHandle;
        //This one is another way, of how to set timers in ue4. By this we can store the function, that we want to call.
        //This is helpful, when the function is outside of this class (It is in TanksPlayerController)
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &AMyPlayerController::SetPlayerEnableState, true);
        int32 Delay = StartDelay + 2;
        GetWorldTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, Delay, false);
    }
}

void ABasicGameMode::HandleGameOver(bool PlayerWon)
{
    GameOver(PlayerWon);
}

/*void ABasicGameMode::PlayerWon()
{
    PlayerControllerRef->SetPlayerEnableState(false);
    //Add some widgets
}

void ABasicGameMode::PlayerLost()
{
    PlayerControllerRef->SetPlayerEnableState(false);
    //Add some widgets
    
}
*/
void ABasicGameMode::StartDecrementing()
{
    GetWorldTimerManager().SetTimer(Decrementing, ScoreDecrement, DecrementDelay, true);
    if(GS->GetScore() <= 0)
    {
        GetWorldTimerManager().ClearTimer(Decrementing);
        PlayerControllerRef->SetPlayerEnableState(false);
        //HandleGameOver(false);
    }
    
}
