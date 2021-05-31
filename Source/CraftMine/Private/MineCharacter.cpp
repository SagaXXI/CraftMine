// Fill out your copyright notice in the Description page of Project Settings.
//Add impulse to character. Impulses are accumulated each tick and applied together so multiple calls to this function will accumulate.
//An impulse is an instantaneous force, usually applied once. If you want to continually apply forces each frame, use AddForce().
//Note that changing the momentum of characters like this can change the movement mode.



//TODO add gamemode with score, when collecting a golden ore
//search for right models
//make the ore destruction clear and nice, by ticking the details and change some parameters
//create a destructible mesh and change destruct it, when you are digging (in Dig() function change the currentore material
//Add a map
//Add digging animation, effects, sound
//Add UI to digging, and crosshair
//Make a camera, that will be above us, and we will be looking to game above

#include "MineCharacter.h"
#include "MyPlayerController.h"
#include "Ore.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMineCharacter::AMineCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
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
	PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

/*void AMineCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}*/

// Called to bind functionality to input
void AMineCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMineCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMineCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMineCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AMineCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Dig"), IE_Pressed, this, &AMineCharacter::OnDigging);
	PlayerInputComponent->BindAction(TEXT("Dig"), IE_Released, this, &AMineCharacter::StopDigging);

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
	if(CurrentOre)
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

AOre* AMineCharacter::GetCurrentOre()
{
	return CurrentOre;
}