// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerFlight.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 3. USphereComponent로 루트 콜리전을 생성한다.
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Col"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(200);


	// 4. UStaticMesh를 루트의 자식 컴포넌트로 생성한다.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(4.0f));

	// 8. 충돌 프리셋을 "TrapPreset" 으로 설정한다.
	sphereComp->SetCollisionProfileName(TEXT("TrapPreset"));

}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
	// 7. overlap 델리게이트에 InTrap 함수를 연결한다.
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::InTrap);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ATrap::OutTrap);
	
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 누적 시간(currentTime)이 지정된 시간(teleportTime)에 도달했다면..
	if (currentTime >= teleportTime)
	{
		// 랜덤한 위치(벡터)를 설정한다.
		//float random_h = FMath::RandRange(-500.0f, 500.0f);
		//float random_v = FMath::RandRange(-300.0f, 300.0f);
		////FVector random_vec = FVector(0, random_h, random_v);
		//FVector random_vec;
		//random_vec.Y = random_h;
		//random_vec.Z = random_v;

		FVector random_vec = FMath::VRand()* 300.0f;
		random_vec.X = 0;

		// 그 위치로 이동한다.
		SetActorLocation(random_vec);

		// currentTime을 다시 초기화한다.
		currentTime = 0;
	}
	// 그렇지 않다면...
	else
	{
		// 시간을 누적한다.
		currentTime += DeltaTime;
	}
}

// 6. "InTrap" 함수 구현
void ATrap::InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Player is mine"));
	// 만일, 플레이어인 경우라면...
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player)
	{
		// 플레이어에 있는 CanFire 변수를 False로 만든다.
		player->canFire = false;
	}
}

void ATrap::OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 만일, 나가기 직전에 부딪힌 액터가 플레이어라면...
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr)
	{
		// 플레이어의 canFire 변수의 값을 true로 변경한다.
		player->canFire = true;

	}

}

