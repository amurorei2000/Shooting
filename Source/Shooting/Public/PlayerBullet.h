// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "PlayerBullet.generated.h"

/**
 *
 */
UCLASS()
class SHOOTING_API APlayerBullet : public ABullet
{
	GENERATED_BODY()

public:
	APlayerBullet();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
