// Fill out your copyright notice in the Description page of Project Settings.
//Add impulse to character. Impulses are accumulated each tick and applied together so multiple calls to this function will accumulate.
//An impulse is an instantaneous force, usually applied once. If you want to continually apply forces each frame, use AddForce().
//Note that changing the momentum of characters like this can change the movement mode.



//TODO Add a map
//Check for a HUD, displaying digging progress
//search for right models
//Add digging animation, effects, sound


#include "MineCharacter.h"
#include "MyPlayerController.h"
#include "Ore.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMineCharacter::AMineCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

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
	PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AMineCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//GetHitUnderCursor() will rotate the turret, according to your cursor, like in Tanki Online. 
	//Detailly, it will just trace to place, under your cursor
	if(PlayerController)
	{
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
		FVector HitLocation = TraceHitResult.Location;
		
		//This will finally rotate the turret, according to the location of trace, which is under your cursor
		RotateMesh(HitLocation);
	}
	
}

// Called to bind functionality to input
void AMineCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMineCharacter::MoveForward);
	PlayerInputComponent->BindAction(TEXT("Dig"), IE_Pressed, this, &AMineCharacter::OnDigging);
	PlayerInputComponent->BindAction(TEXT("Dig"), IE_Released, this, &AMineCharacter::StopDigging);
	PlayerInputComponent->BindAction(TEXT("Running"), IE_Pressed, this, &AMineCharacter::Running);
	PlayerInputComponent->BindAction(TEXT("Running"), IE_Released, this, &AMineCharacter::StopRunning);
}

void AMineCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMineCharacter::Running()
{
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
}

void AMineCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMineCharacter::CheckIfOre()
{
	FHitResult TraceResult;

	FVector Start = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation(); 
	FVector End = (UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector() * TraceRange) + Start;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if(GetWorld()->LineTraceSingleByChannel(TraceResult, TraceHitResult.TraceStart, TraceHitResult.TraceEnd, ECollisionChannel::ECC_WorldDynamic, Params))
	{
		AOre* Ore = Cast<AOre>(TraceResult.GetActor());
		if(Ore)
		{
			CurrentOre = Ore;
			UE_LOG(LogTemp, Warning, TEXT("%s found"), *Ore->GetName())
		}
		else
		{
			CurrentOre = nullptr;
		}
	}
}

void AMineCharacter::OnDigging()
{
	//PlayDigAnimation();
	CheckIfOre();
	if(CurrentOre && CurrentOre->IsPlayerOverlapped())
	{
		bIsDiggingNow = true;
		GetWorld()->GetTimerManager().SetTimer(DiggingTimer,this, &AMineCharacter::HoldDig, DiggingDelay, true, 0.f);
	}
}

void AMineCharacter::HoldDig()
{
	
	UGameplayStatics::ApplyDamage(CurrentOre, DiggingDamage, PlayerController, this, DamageType);
}

void AMineCharacter::StopDigging()
{
	GetWorld()->GetTimerManager().ClearTimer(DiggingTimer);
	bIsDiggingNow = false;
	if(CurrentOre)
	{
		if(!CurrentOre->GetIsDestroyed())
		{
			CurrentOre->Heal(1000.f, PlayerController, this);
		}
	}
}

float AMineCharacter::GetOreCurrentHealth()
{
	return CurrentOre->GetCurrentHealth();
}

float AMineCharacter::GetOreMaxHealth()
{
	return CurrentOre->GetMaxHealth();
}

AOre* AMineCharacter::GetCurrentOre()
{
	return CurrentOre;
}

//Will rotate the turret, to where we look, like in World of Tanks
void AMineCharacter::RotateMesh(FVector LookAtTarget)
{
	FVector StartLocation = GetActorLocation();
	
	//Find a rotation for an object at Start location to point at Target location.
	FRotator MeshRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, FVector(LookAtTarget.X, LookAtTarget.Y, GetActorLocation().Z));
	SetActorRotation(MeshRotation);

}
