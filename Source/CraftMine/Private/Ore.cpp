// Fill out your copyright notice in the Description page of Project Settings.


#include "Ore.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"
#include "MineCharacter.h"

// Sets default values
AOre::AOre()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Mesh"));
	//Same thing as notify component hit events
	//Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetupAttachment(RootComponent);
	

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Comp"));
	CollisionBox->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AOre::BeginPlay()
{
	Super::BeginPlay();

	//Mesh->OnComponentHit.AddDynamic(this, &AOre::Damage);

	this->OnTakeAnyDamage.AddDynamic(this, &AOre::Damage);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AOre::Trigger);

	CurrentHealth = MaxHealth;
	
}

void AOre::Damage(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);
	AMineCharacter* Player = Cast<AMineCharacter>(DamageCauser);
	if(!Player) return;
	if(!bIsDestroyed && Player->bIsDiggingNow)
	{
		Mesh->AddImpulse(Mesh->GetForwardVector() * ImpulseForce * Mesh->GetMass(), NAME_None);
		CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
		if(CurrentHealth == 0.f)
		{
			DestroyActor(CurrentHealth, GetActorLocation(),Mesh->GetForwardVector() , ImpulseForce);
		}
	}
	
}

void AOre::Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AOre::DestroyActor(float DamageAmount, FVector HitLocation, FVector ImpulseDirection, float Impulse)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);
	if(!bIsDestroyed)
	{
		bIsDestroyed = true;
		Mesh->ApplyDamage(CurrentHealth, HitLocation, ImpulseDirection, Impulse);
	}
}

// Called every frame
void AOre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

