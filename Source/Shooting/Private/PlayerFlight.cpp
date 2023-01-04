// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Enemy.h"
#include "MyShootingGameModeBase.h"

APlayerFlight::APlayerFlight()
{
	PrimaryActorTick.bCanEverTick = true;

	// �浹 �ڽ� ������Ʈ�� �����Ѵ�.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	// ������ �浹 �ڽ� ������Ʈ�� ��Ʈ ������Ʈ�� �����Ѵ�.
	SetRootComponent(boxComp);
	
	// �ڽ� �ݸ����� ũ�⸦ ����x����x���� ��� 50��Ƽ���ͷ� �����Ѵ�.
	boxComp->SetBoxExtent(FVector(50.0f));

	// �ڽ� �ݸ����� �浹 ó�� �������� "PlayerPreset"���� �����Ѵ�.
	boxComp->SetCollisionProfileName(TEXT("PlayerPreset"));

	// �޽� ������Ʈ�� �����Ѵ�.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	// �޽� ������Ʈ�� ��Ʈ ������Ʈ�� ���� ��ü�� ����Ѵ�.
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �޽� ������Ʈ�� static mesh �׸� ť�� ������ �Ҵ��Ѵ�.
	ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	// ����, ť�� ������ �ε��ϴµ� �����ߴٸ�...
	if (cubeMesh.Succeeded())
	{
		// �ε��� ������ �޽� ������Ʈ�� static mesh �׸� �ִ´�.
		meshComp->SetStaticMesh(cubeMesh.Object);
	}

}

void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* playerCon = Cast<APlayerController>(GetController());

	if (playerCon != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

		if (subsys != nullptr)
		{
			subsys->AddMappingContext(imc_myMapping, 0);
		}
	}

	// ���� ���� ���� �����Ѵ�.
	UMaterialInterface* iMat = meshComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("myColor"));
	
	// Material Interface���� ���� �Ķ���� ���� initColor ������ �����Ѵ�.
	iMat->GetVectorParameterValue(param, initColor);

	UE_LOG(LogTemp, Warning, TEXT("R: %f, G: %f, B:%f"), initColor.R, initColor.G, initColor.B);

	// Material Interface�� �̿��ؼ� Material Instance Dynamic ��ü�� �����.
	dynamicMat = UMaterialInstanceDynamic::Create(iMat, this);

	// ������ ���̳��� ���͸����� �޽ÿ� �����Ѵ�.
	if (dynamicMat != nullptr)
	{
		meshComp->SetMaterial(0, dynamicMat);
	}
}

void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ����ڰ� �Է��� ������ �̵��ϰ� �ʹ�.
	// ������ ����ȭ
	direction.Normalize();

	// P = P0 + vt
	FVector dir = GetActorLocation() + direction * moveSpeedOrigin * DeltaTime;
	SetActorLocation(dir, true);

}

void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// ������ UInputComponent*�� UEnhancedInputComponent*�� ��ȯ�Ѵ�.
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// ��ȯ�� enhancedInputComponent�� ��� �Լ� BindAction�� �̿��ؼ� �Է��� ���ε��Ѵ�.
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);
	enhancedInputComponent->BindAction(ia_boost, ETriggerEvent::Started, this, &APlayerFlight::Boost);
	enhancedInputComponent->BindAction(ia_boost, ETriggerEvent::Completed, this, &APlayerFlight::Boost);
	enhancedInputComponent->BindAction(ia_ULT, ETriggerEvent::Triggered, this, &APlayerFlight::ExplosionAll);

	//// Horizontal Axis �Է¿� �Լ��� �����Ѵ�.
	//PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);
	//
	//// Vertical Axis �Է¿� �Լ��� �����Ѵ�.
	//PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical);

	//// Fire Action �Է¿� �Լ��� �����Ѵ�.
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerFlight::FireBullet);
}

void APlayerFlight::ReservationHitColor(float time)
{
	// 1. ������ Red ����� �����Ѵ�.
	dynamicMat->SetVectorParameterValue(TEXT("myColor"), (FVector4)FLinearColor::Red);

	// 2. ���� �������� �ǵ����� �Լ��� ���ε��� Ÿ�̸Ӹ� �����Ѵ�.
	GetWorld()->GetTimerManager().SetTimer(colorTimer, this, &APlayerFlight::ChangeOriginColor, time, false);
}


void APlayerFlight::ChangeOriginColor()
{
	dynamicMat->SetVectorParameterValue(TEXT("myColor"), (FVector4)initColor);
}

// �¿� �Է��� ���� �� ����� �Լ�
//void APlayerFlight::Horizontal(float value)
//{
//	h = value;
//	//UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
//	direction.Y = h;
//}

void APlayerFlight::Horizontal(const FInputActionValue& value)
{
	h = value.Get<float>();
	//UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
	direction.Y = h;
}

// ���� �Է��� ���� �� ����� �Լ�
//void APlayerFlight::Vertical(float value)
//{
//	v = value;
//	//UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
//	direction.Z = v;
//}

void APlayerFlight::Vertical(const FInputActionValue& value)
{
	v = value.Get<float>();
	//UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
	direction.Z = v;
}

// ���콺 ���� ��ư�� ������ �� ����� �Լ�
void APlayerFlight::FireBullet()
{
	if (!canFire)
	{
		return;
	}

	// �Ѿ��� �����Ѵ�.
	// �Ѿ� �������Ʈ ����
	// �Ѿ��� bulletCount ����ŭ ���ÿ� �߻��Ѵ�.

	for (int32 i = 0; i < bulletCount; i++)
	{
		// �Ѿ��� ��ü ����
		float totalSize = (bulletCount - 1) * bulletSpacing;

		// ���� ��ġ
		float base_y = totalSize * -0.5f;

		// ���� ��ġ�� ������ ���͸� �����.
		FVector offset = FVector(0, base_y + 150 * i, 0);

		FVector spawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
		// �����¸�ŭ ��ġ�� �����Ѵ�.
		spawnPosition += offset;
		FRotator spawnRotation = FRotator(90.0f, 0, 0);
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, spawnPosition, spawnRotation, param);
		
		//������ �Ѿ�(bullet)�� bulletAngle ��ŭ �����ϰ� ȸ����Ų��.
		float base_yaw = (bulletCount - 1) * bulletAngle * -0.5f;
		FRotator rot_base = FRotator(0, base_yaw + bulletAngle * i, 0);

		bullet->AddActorLocalRotation(rot_base);

	}
	// �Ѿ� �߻� ȿ������ �����Ѵ�.
	UGameplayStatics::PlaySound2D(this, fireSound);
}

void APlayerFlight::StartBoost()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Boost!"));
	moveSpeedOrigin = moveSpeed * 2;
}

void APlayerFlight::EndBoost()
{
	UE_LOG(LogTemp, Warning, TEXT("End Boost!"));
	moveSpeedOrigin = moveSpeed;
}

void APlayerFlight::Boost()
{
	isBoosting = !isBoosting;

	//if (isBoosting)
	//{
	//	moveSpeedOrigin = moveSpeed * 2;
	//}
	//else
	//{
	//	moveSpeedOrigin = moveSpeed;
	//}

	moveSpeedOrigin = isBoosting ? moveSpeed * 2 : moveSpeed;
}

// �ñر� ��ź �Լ�
void APlayerFlight::ExplosionAll()
{
	// ��� Enemy�� �ı��Ѵ�.

	// 1. TActorIterator<T>�� �̿��� ���
	/*for (TActorIterator<AEnemy> enemy(GetWorld()); enemy; ++enemy)
	{
		enemy->DestroyMySelf();
	}*/

	// 2. TArray<T> �迭�� �̿��� ���
	AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	TArray<AEnemy*> testArr = gm->enemies;

	if (gm != nullptr)
	{
		for (int32 i = 0; i < testArr.Num(); i++)
		{
			// Pending Kill ���� üũ
			if (IsValid(testArr[i]))
			{
				testArr[i]->DestroyMySelf();
			}
		}

		// ����Ʈ�� �� �� �ʱ�ȭ�Ѵ�.
		gm->enemies.Empty();
	}
	
}
