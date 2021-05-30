// Fill out your copyright notice in the Description page of Project Settings.


#include "Ore.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"
#include "MineCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOre::AOre()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Mesh"));
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
		if(Damage > 0.f)
		{
			UE_LOG(LogTemp, Error, TEXT("jdfjd"))
		}
		
		FVector ImpulseDir = Mesh->GetForwardVector();
		Mesh->ApplyDamage(Damage, GetActorLocation(), ImpulseDir,ImpulseForce);
		CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
		if(CurrentHealth == 0.f)
		{
			//Do here things, like picking the gold up in your inventory 
			UE_LOG(LogTemp, Warning, TEXT("Vse"))
			DestroyActor(MaxHealth * 100, GetActorLocation(), DamageExplosionRadius * 10, ImpulseForce, true);
			SetLifeSpan(BPDestroyTime);
		}
	}
	
}

void AOre::DestroyActor(float BaseDamage, const FVector& HurtOrigin, float DamageRad, float ImpulseStrength, bool bFullDamage)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);
	if(!bIsDestroyed)
	{
		bIsDestroyed = true;
		Mesh->ApplyRadiusDamage(BaseDamage, HurtOrigin, DamageExplosionRadius, ImpulseStrength, bFullDamage);
	}
}

void AOre::Heal(float HP, AController* PlayerController, AActor* HealMaker)
{
    CurrentHealth = FMath::Clamp(CurrentHealth + HP, CurrentHealth, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Healed with %f points"), CurrentHealth)
}

bool AOre::GetIsDestroyed()
{
	return bIsDestroyed;
}



