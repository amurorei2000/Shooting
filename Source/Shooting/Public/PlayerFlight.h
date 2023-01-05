// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerFlight.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FULTbomb);

// 1. 벡터를 인자로 넘겨받는 델리게이트 선언(FDirectionModifier)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDirectionModifier, FVector, newDir);


UCLASS()
class SHOOTING_API APlayerFlight : public APawn
{
	GENERATED_BODY()

public:
	APlayerFlight();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerSettings)
	float moveSpeed = 500;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	class UInputAction* ia_horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	class UInputAction* ia_vertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	class UInputAction* ia_fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	class UInputAction* ia_boost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	class UInputAction* ia_ULT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	class UInputMappingContext* imc_myMapping;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class USoundBase* fireSound;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	int32 bulletCount = 1;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float bulletSpacing = 150;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float bulletAngle = 30;

	void ReservationHitColor(float time);
	void ChangeOriginColor();

	bool canFire = true;

	FULTbomb playerBomb;
	FDirectionModifier OnSetDirection;

private:
	//void Horizontal(float value);
	//void Vertical(float value);

	void Horizontal(const FInputActionValue& value);
	void Vertical(const FInputActionValue& value);
	void FireBullet();
	void StartBoost();
	void EndBoost();
	void Boost();

	UFUNCTION()
	void ExplosionAll();

	bool isBoosting = false;

	float moveSpeedOrigin = 500;
	float h;
	float v;
	FVector direction;
	FLinearColor initColor;
	FTimerHandle colorTimer;
	UMaterialInstanceDynamic* dynamicMat;
};
