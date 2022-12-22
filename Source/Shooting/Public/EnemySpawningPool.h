// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawningPool.generated.h"

UCLASS()
class SHOOTING_API AEnemySpawningPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawningPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category=PoolSettings)
	float spawnCool = 2.0f;

	UPROPERTY(EditInstanceOnly, Category = PoolSettings)
	TSubclassOf<class AEnemy> enemyFactory;



	UPROPERTY(EditInstanceOnly, Category = PoolSettings)
	class UArrowComponent* spawnArrow;

	UPROPERTY(EditInstanceOnly, Category = PoolSettings)
	class USceneComponent* rootScene;

private:
	float currentTime = 0;
};
