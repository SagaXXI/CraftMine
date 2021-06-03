// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::SetPlayerEnableState(bool State)
{
	
	if(State)
	{
		GetPawnOrSpectator()->EnableInput(this);
		AMyPlayerController::bShowMouseCursor = true;
		
	}
	else
	{
		GetPawnOrSpectator()->DisableInput(this);
		AMyPlayerController::bShowMouseCursor = false;
	}
}
