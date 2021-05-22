// Fill out your copyright notice in the Description page of Project Settings.


#include "MineCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMineCharacter::AMineCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	

}

// Called when the game starts or when spawned
void AMineCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMineCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMineCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMineCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMineCharacter::MoveRight);

}

void AMineCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AMineCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}