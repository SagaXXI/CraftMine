// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <stdbool.h>

#include "GameFramework/Actor.h"
#include "Ore.generated.h"

class UBoxComponent;
class UDestructibleComponent;

UCLASS()
class CRAFTMINE_API AOre : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOre();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Destructible")
	UDestructibleComponent * Mesh;

	UPROPERTY(EditAnywhere, Category = "Destructible")
	UBoxComponent* CollisionBox;

	UFUNCTION()
	void Damage(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void DestroyActor(float DamageAmount, FVector HitLocation, FVector ImpulseDirection, float Impulse);

	bool bIsDestroyed = false;

	bool bIsTriggerEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Destructible")
	float MaxHealth = 10.f;

	float CurrentHealth;
	
	UPROPERTY(EditAnywhere, Category = "Destructible")
	float DamageByHit = 1.f;

	UPROPERTY(EditAnywhere, Category = "Destructible")
	float DefaultImpulse = 1.f;	
};
