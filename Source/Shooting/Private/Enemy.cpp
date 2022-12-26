// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerFlight.h"
#include "EngineUtils.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f));
	// Collision Enabled 값을 Query and Physics로 설정한다.
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 응답 채널의 Object Type을 "Enemy"로 설정한다.
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);

	// 응답 채널을 일괄적으로 Ignore 상태로 처리한다.
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	// 응답 채널을 Player와 bullet 채널에 대해서만 overlap으로 처리한다.
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 추첨을 해서 하나는 정면 방향, 또 다른 하나는 플레이어 방향으로 이동하고 싶다.
	// 1. 추첨을 한다. 확률은 변수를 이용해서 70:30 비율로 한다.
	int32 drawNumber = FMath::RandRange(1, 100);
	
	// 2. 만일, 뽑은 값이 traceRate 보다 작으면...
	if (drawNumber <= traceRate)
	{
		// 2-1. 플레이어 액터를 찾는다.
		
		// 월드에서 특정한 객체를 찾는 방법 - 1
		//AActor* target = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerFlight::StaticClass());
		
		// 월드에서 특정한 객체를 찾는 방법 - 2
		
		for (TActorIterator<APlayerFlight> it(GetWorld()); it; ++it)
		{
			target = *it;
		}

		if (target != nullptr)
		{
			float temp = target->moveSpeed;

			// 2-2. 플레이어의 위치 - 나의 위치 = 갈 방향을 설정한다.
			FVector targetDir = target->GetActorLocation() - GetActorLocation();
			targetDir.Normalize();
			direction = targetDir;
		}
		
	}
	// 3. 그렇지 않으면...
	else
	{
		// 3-1. 정면으로 방향을 정한다.
		direction = GetActorForwardVector();
	}

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	boxComp->SetGenerateOverlapEvents(true);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr)
	{
		//player->Destroy();
		player->ReservationHitColor(0.2f);
		
		Destroy();
	}
}

