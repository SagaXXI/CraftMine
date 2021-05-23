// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Pickaxe.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MineCharacter.generated.h"

class APickaxe;

UCLASS()
class CRAFTMINE_API AMineCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMineCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	//Movement stuff
	void MoveForward(float Value);

	void MoveRight(float Value);

	//Mouse looking stuff
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	
	void LookUp(float AxisValue);

	void LookRight(float AxisValue);

	UPROPERTY(EditAnywhere)
	float MovementSpeed = 500.f;

	UPROPERTY(EditAnywhere)
	float MouseSensitivity = 10.f;

	//Pickaxe spawning stuff
	UPROPERTY(EditAnywhere)
	TSubclassOf<APickaxe> PickaxeToSpawn;

	APickaxe* Pickaxe;
	
	
	
	
	

	

};
