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

	// 3. USphereComponent�� ��Ʈ �ݸ����� �����Ѵ�.
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Col"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(200);


	// 4. UStaticMesh�� ��Ʈ�� �ڽ� ������Ʈ�� �����Ѵ�.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(4.0f));

	// 8. �浹 �������� "TrapPreset" ���� �����Ѵ�.
	sphereComp->SetCollisionProfileName(TEXT("TrapPreset"));

}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
	// 7. overlap ��������Ʈ�� InTrap �Լ��� �����Ѵ�.
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::InTrap);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ATrap::OutTrap);
	
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� ���� �ð�(currentTime)�� ������ �ð�(teleportTime)�� �����ߴٸ�..
	if (currentTime >= teleportTime)
	{
		// ������ ��ġ(����)�� �����Ѵ�.
		//float random_h = FMath::RandRange(-500.0f, 500.0f);
		//float random_v = FMath::RandRange(-300.0f, 300.0f);
		////FVector random_vec = FVector(0, random_h, random_v);
		//FVector random_vec;
		//random_vec.Y = random_h;
		//random_vec.Z = random_v;

		FVector random_vec = FMath::VRand()* 300.0f;
		random_vec.X = 0;

		// �� ��ġ�� �̵��Ѵ�.
		SetActorLocation(random_vec);

		// currentTime�� �ٽ� �ʱ�ȭ�Ѵ�.
		currentTime = 0;
	}
	// �׷��� �ʴٸ�...
	else
	{
		// �ð��� �����Ѵ�.
		currentTime += DeltaTime;
	}
}

// 6. "InTrap" �Լ� ����
void ATrap::InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Player is mine"));
	// ����, �÷��̾��� �����...
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player)
	{
		// �÷��̾ �ִ� CanFire ������ False�� �����.
		player->canFire = false;
	}
}

void ATrap::OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ����, ������ ������ �ε��� ���Ͱ� �÷��̾���...
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr)
	{
		// �÷��̾��� canFire ������ ���� true�� �����Ѵ�.
		player->canFire = true;

	}

}

