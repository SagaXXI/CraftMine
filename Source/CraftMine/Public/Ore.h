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

	bool GetIsDestroyed();
	
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
	
	void DestroyActor(float BaseDamage, const FVector& HurtOrigin, float DamageRad, float ImpulseStrength, bool bFullDamage);

	bool bIsDestroyed = false;

	UPROPERTY(EditAnywhere, Category = "Destructible")
	float MaxHealth = 10.f;

	float CurrentHealth;


	/*UPROPERTY(EditAnywhere, Category = "Destructible")
	float MeshMaxHealth = 10.f;
	
	float MeshHealth;*/
	
	UPROPERTY(EditAnywhere, Category = "Destructible")
	float DamageByHit = 1.f;

	UPROPERTY(EditAnywhere, Category = "Destructible")
	float ImpulseForce = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = "Destructible")
	float DamageRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Destructible")
	float BPDestroyTime = 7.f;
	
};
