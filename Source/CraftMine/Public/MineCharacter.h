// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Pickaxe.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MineCharacter.generated.h"



class APickaxe;
class AOre;
class AMyPlayerController;

UCLASS()
class CRAFTMINE_API AMineCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMineCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaSeconds) override;

	//For animation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDiggingNow = false;

	//for widgets in blueprints
	UFUNCTION(BlueprintCallable)
	float GetOreCurrentHealth();

	//For basicgamemode class in c++
	AOre* GetCurrentOre();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digging")
	float DiggingDamage = 2.f;
private:
	/////////////////////////
	//Movement stuff
	void MoveForward(float Value);

	/////////////////////////
	//Mouse looking stuff
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	
	void RotateMesh(FVector LookAtTarget);

	FHitResult TraceHitResult;

	/////////////////////////
	//Pickaxe spawning stuff
	UPROPERTY(EditAnywhere, Category = "Digging")
	TSubclassOf<APickaxe> PickaxeToSpawn;

	APickaxe* Pickaxe;

	//Checking if we are looking forward to an ore
	void CheckIfOre();

	UPROPERTY(EditAnywhere, Category = "Digging")
	float TraceRange = 1000.f;

	/////////////////////////
	//Digging mechanics

	//The function that will be firing Dig() function, animation, and digging delay
	void OnDigging ();

	//The actual digging function, that will destroy the ore, and collect it
	void HoldDig();

	//Stops playing digging animation
	void StopDigging();

	//Current ore that we are looking now
	AOre* CurrentOre;
	
	//Delay used for digging
	UPROPERTY(EditAnywhere, Category = "Digging")
	float DiggingDelay = 1.f;
	
	//Time, in which we were digging
	FTimerHandle DiggingTimer;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	TSubclassOf<UDamageType> DamageType;

	AMyPlayerController* PlayerController;




	


	
	

	

};
