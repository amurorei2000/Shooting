// Fill out your copyright notice in the Description page of Project Settings.


#include "PointerTest.h"

// Sets default values
APointerTest::APointerTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	myAge = &realAge;
}

// Called when the game starts or when spawned
void APointerTest::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void APointerTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

