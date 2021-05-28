// Fill out your copyright notice in the Description page of Project Settings.


#include "Ore.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"
#include "MineCharacter.h"

// Sets default values
AOre::AOre()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	this->OnTakeAnyDamage.AddDynamic(this, &AOre::Damage);

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
		Mesh->ApplyRadiusDamage(Damage, GetActorLocation(), DamageRadius, ImpulseForce, true);
		CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
		if(CurrentHealth == 0.f)
		{
			//Do here things, like picking the gold up in your inventory 
			UE_LOG(LogTemp, Warning, TEXT("Vse"))
			DestroyActor(MaxHealth * 100, GetActorLocation(), DamageRadius * 10, ImpulseForce, true);
		}
	}
	
}

void AOre::DestroyActor(float BaseDamage, const FVector& HurtOrigin, float DamageRad, float ImpulseStrength, bool bFullDamage)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);
	if(!bIsDestroyed)
	{
		bIsDestroyed = true;
		Mesh->ApplyRadiusDamage(BaseDamage, HurtOrigin, DamageRadius, ImpulseStrength, bFullDamage);
	}
}

