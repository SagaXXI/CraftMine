// Fill out your copyright notice in the Description page of Project Settings.

//TODO fix the mouse rotation che za huinuya
//add a pickaxe
//create the golden and copper ore
//add digging mechanics
//add gamemode with score
//search for right models

#include "MineCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMineCharacter::AMineCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
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
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMineCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AMineCharacter::LookRight);

}

void AMineCharacter::MoveRight(float Value)
{
	if(Controller != NULL && Value != NULL)
	{
		const FRotator Rotation = Controller->GetControlRotation(); 
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMineCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue * MouseSensitivity);
}

void AMineCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * MouseSensitivity);
}

void AMineCharacter::MoveForward(float Value)
{
	if(Controller != NULL && Value != NULL)
	{
		const FRotator Rotation = Controller->GetControlRotation(); 
		const FRotator PitchRotation(Rotation.Pitch, 0, 0);
		const FVector Direction = FRotationMatrix(PitchRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
	
}
