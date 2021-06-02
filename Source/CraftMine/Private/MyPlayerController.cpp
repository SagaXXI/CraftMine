// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::SetPlayerEnableState(bool State)
{
	AMyPlayerController::bShowMouseCursor = false;
	if(State)
	{
		GetPawnOrSpectator()->EnableInput(this);
		
	}
	else
	{
		GetPawnOrSpectator()->DisableInput(this);
	}
}
