// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerFlight.h"
#include "Enemy.h"

// Sets default values
AShieldActor::AShieldActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("box collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 150, 50));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh Comp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(1.0f, 3.0f, 1.0f));

	// 플레이어랑 에너미 채널하고만 충돌 응답(overlap) 처리하기
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxComp->SetGenerateOverlapEvents(true);

	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AShieldActor::BeginPlay()
{
	Super::BeginPlay();
	
	
	
	// 충돌시 실행할 함수 바인딩하기
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AShieldActor::ShieldOverlapped);

	//UE_LOG(LogTemp, Warning, TEXT("000000000"));
}

// Called every frame
void AShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AddActorWorldRotation(FRotator(0, 0, 3));
	//SetActorRotation(FRotator(0, 0, 3));
}

void AShieldActor::ShieldOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("111111111111111111111111111111"));

	// 부딪힌 대상이 플레이어라면....
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	AEnemy* enemy = Cast<AEnemy>(OtherActor);
	
	if (player != nullptr)
	{
		// 플레이어에게 부착된다.

		this->AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform);
		//player->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		FTimerHandle snapTimer;
		GetWorld()->GetTimerManager().SetTimer(snapTimer, this, &AShieldActor::ResetAttach, 3, false);

	}
	// 부딪힌 대상이 에너미라면...
	else if (enemy != nullptr)
	{
		// 충돌한 에너미를 파괴한다.
		enemy->Destroy();

		//UE_LOG(LogTemp, Warning, TEXT("22222222222222222222222222"));
	}
}

void AShieldActor::ResetAttach()
{
	// 붙여놨던 부모를 해제한다.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

