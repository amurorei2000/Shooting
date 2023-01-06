// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingGameModeBase.h"




// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 영역 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	boxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));
	

	// 메쉬 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeLocation(FVector(0, 0, -50.0f));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// 충돌(오버랩)이 발생하면 실행할 함수를 연결한다.
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);

	// 오버랩 이벤트가 발생하도록 설정한다.
	boxComp->SetGenerateOverlapEvents(true);

	// 태어난 후 2초 뒤에 제거한다.
	//SetLifeSpan(2.0f);

	GetWorld()->GetTimerManager().SetTimer(lifeTimer, this, &ABullet::DestroyMySelf, 2.0f, false);

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 나의 정면 방향으로 계속 이동하고 싶다.
	//direction = FVector(1, 0, 0);
	direction = GetActorForwardVector();

	SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);

}



void ABullet::SetDirection(FVector value)
{
	direction = value;
}

void ABullet::DestroyMySelf()
{
	Destroy();
}