// Fill out your copyright notice in the Description page of Project Settings.


#include "BossActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyBullet.h"
#include "Kismet/KismetMathLibrary.h"

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

	if (currentTime > pattern_delay)
	{
		BossAttack1(15, 6);
		currentTime = 0;
	}
	else
	{
		currentTime += DeltaTime;
	}

}

void ABossActor::BossAttack1(float angle, int32 count)
{
	// angle 간격으로 count 수만큼 총알을 생성한다.
	UE_LOG(LogTemp, Warning, TEXT("Fire !!!"));

	// 시작 각도 = 270 - (angle * (count-1) * 0.5)
	float startAngle = 270 - (angle * (count - 1) * 0.5f);

	for (int32 i = 0; i < count; i++)
	{
		// 기본 좌표 (0, rcos@ , rsin@)
		FVector bullet_base = FVector(0,
					50 * FMath::Cos(FMath::DegreesToRadians(startAngle + angle * i)),
					50 * FMath::Sin(FMath::DegreesToRadians(startAngle + angle * i)));

		/*UE_LOG(LogTemp, Warning, TEXT("%f"), FMath::Sin(FMath::DegreesToRadians(90)));*/
		
		AEnemyBullet* enemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(bullet, GetActorLocation() + bullet_base, FRotator(-90, 0, 0));
		
		//enemyBullet->SetDirection(bullet_base.GetSafeNormal());
		
		//  Up Vector 축을 bullet_base 방향으로 회전시켰을 때의 로테이터 값을 계산한다.
		FRotator rot = UKismetMathLibrary::MakeRotFromZX(enemyBullet->GetActorUpVector(),
														bullet_base.GetSafeNormal());
		enemyBullet->SetActorRotation(rot);
		//enemyBullet->SetDirection(enemyBullet->GetActorForwardVector());
	}

}

