// Fill out your copyright notice in the Description page of Project Settings.

//TODO add digging mechanics
//add gamemode with score
//search for right models

#include "MineCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ore.h"


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
	//Spawning a pickaxe
	Pickaxe = GetWorld()->SpawnActor<APickaxe>(PickaxeToSpawn);
	//Attaches pickaxe to choosen socket in skeletal mesh
	Pickaxe->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("PickaxeSocket"));
	Pickaxe->SetOwner(this);
}

// Called every frame
void AMineCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckIfOre();

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
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
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

void AMineCharacter::CheckIfOre()
{
	FHitResult TraceResult;

	FVector Start = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation(); 
	FVector End = (UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector() * TraceRange) + Start;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if(GetWorld()->LineTraceSingleByChannel(TraceResult, Start, End, ECollisionChannel::ECC_WorldDynamic, Params))
	{
		AOre* Ore = Cast<AOre>(TraceResult.GetActor());
		if(Ore)
		{
			CurrentOre = Ore;
			UE_LOG(LogTemp, Warning, TEXT("%s found"), *Ore->GetName())
			return;
		}
		else
		{
			CurrentOre = nullptr;
			return;
		}
	}
}

void AMineCharacter::MoveForward(float Value)
{
	if(Controller != NULL && Value != NULL)
	{
		const FRotator Rotation = Controller->GetControlRotation(); 
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
