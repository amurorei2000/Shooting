// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossActor.generated.h"

UCLASS()
class SHOOTING_API ABossActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = BossSettings)
	class UBoxComponent* boxCollision;

	UPROPERTY(EditAnywhere, Category = BossSettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = BossSettings)
	float testValue;

	UPROPERTY(EditAnywhere, Category = BossSettings)
	TSubclassOf<class AEnemyBullet> bullet;

private:
	FVector startLocation;
	FVector endLocation;

	float currentTime = 0;
	float pattern_delay = 2.0f;

	UFUNCTION()
	void BossAttack1(float angle, int32 count);

};
