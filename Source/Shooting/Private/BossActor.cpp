// Fill out your copyright notice in the Description page of Project Settings.


#include "BossActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABossActor::ABossActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(boxCollision);
	boxCollision->SetBoxExtent(FVector(50, 300, 150));
	boxCollision->SetGenerateOverlapEvents(true);
	boxCollision->SetCollisionProfileName(TEXT("Boss"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(1, 6, 3));
}

// Called when the game starts or when spawned
void ABossActor::BeginPlay()
{
	Super::BeginPlay();
	
	startLocation = GetActorLocation();
}

// Called every frame
void ABossActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#pragma region MyRegion



	// 현재 위치에서 (0, 0, 200) 위치까지 이동한다.
	//float distance = (startLocation - GetActorLocation()).Length();
	//float shaking = FMath::RandRange(-1.0f, 1.0f);

	//if (distance < 500)
	//{
	//	FVector dir = GetActorUpVector() * -1;
	//	dir.Y = shaking;
	//	SetActorLocation(GetActorLocation() + dir * 200 * DeltaTime);
	//}
	//else
	//{
	//	SetActorLocation(FVector(0, 0, 200));
	//}
#pragma endregion

	testValue += DeltaTime * 0.5f;
	testValue = FMath::Clamp(testValue, 0.0f, 1.0f);

	FVector result = FMath::Lerp(startLocation, endLocation, testValue);
	SetActorLocation(result);
}

