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

	// 충돌 박스 컴포넌트를 생성한다.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	// 생성된 충돌 박스 컴포넌트를 루트 컴포넌트로 설정한다.
	SetRootComponent(boxComp);
	
	// 박스 콜리전의 크기를 가로x세로x높이 모두 50센티미터로 설정한다.
	boxComp->SetBoxExtent(FVector(50.0f));

	// 박스 콜리전의 충돌 처리 프리셋을 "PlayerPreset"으로 설정한다.
	boxComp->SetCollisionProfileName(TEXT("PlayerPreset"));

	// 메시 컴포넌트를 생성한다.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	// 메시 컴포넌트를 루트 컴포넌트의 하위 개체로 등록한다.
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 메시 컴포넌트의 static mesh 항목에 큐브 파일을 할당한다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	// 만일, 큐브 파일을 로드하는데 성공했다면...
	if (cubeMesh.Succeeded())
	{
		// 로드한 파일을 메시 컴포넌트의 static mesh 항목에 넣는다.
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

	// 현재 색상 값을 저장한다.
	UMaterialInterface* iMat = meshComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("myColor"));
	
	// Material Interface에서 벡터 파라미터 값을 initColor 변수에 저장한다.
	iMat->GetVectorParameterValue(param, initColor);

	UE_LOG(LogTemp, Warning, TEXT("R: %f, G: %f, B:%f"), initColor.R, initColor.G, initColor.B);

	// Material Interface를 이용해서 Material Instance Dynamic 개체를 만든다.
	dynamicMat = UMaterialInstanceDynamic::Create(iMat, this);

	// 생성한 다이나믹 매터리얼을 메시에 설정한다.
	if (dynamicMat != nullptr)
	{
		meshComp->SetMaterial(0, dynamicMat);
	}
}

void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 사용자가 입력한 방향대로 이동하고 싶다.
	// 벡터의 정규화
	direction.Normalize();

	// P = P0 + vt
	FVector dir = GetActorLocation() + direction * moveSpeedOrigin * DeltaTime;
	SetActorLocation(dir, true);

}

void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// 기존의 UInputComponent*를 UEnhancedInputComponent*로 변환한다.
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// 변환된 enhancedInputComponent의 멤버 함수 BindAction을 이용해서 입력을 바인딩한다.
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);
	enhancedInputComponent->BindAction(ia_boost, ETriggerEvent::Started, this, &APlayerFlight::Boost);
	enhancedInputComponent->BindAction(ia_boost, ETriggerEvent::Completed, this, &APlayerFlight::Boost);
	enhancedInputComponent->BindAction(ia_ULT, ETriggerEvent::Triggered, this, &APlayerFlight::ExplosionAll);

	//// Horizontal Axis 입력에 함수를 연결한다.
	//PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);
	//
	//// Vertical Axis 입력에 함수를 연결한다.
	//PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical);

	//// Fire Action 입력에 함수를 연결한다.
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerFlight::FireBullet);
}

void APlayerFlight::ReservationHitColor(float time)
{
	// 1. 색상을 Red 색깔로 변경한다.
	dynamicMat->SetVectorParameterValue(TEXT("myColor"), (FVector4)FLinearColor::Red);

	// 2. 원래 색상으로 되돌리는 함수를 바인딩한 타이머를 예약한다.
	GetWorld()->GetTimerManager().SetTimer(colorTimer, this, &APlayerFlight::ChangeOriginColor, time, false);
}


void APlayerFlight::ChangeOriginColor()
{
	dynamicMat->SetVectorParameterValue(TEXT("myColor"), (FVector4)initColor);
}

// 좌우 입력이 있을 때 실행될 함수
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

// 상하 입력이 있을 때 실행될 함수
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

// 마우스 왼쪽 버튼을 눌렀을 때 실행될 함수
void APlayerFlight::FireBullet()
{
	if (!canFire)
	{
		return;
	}

	// 총알을 생성한다.
	// 총알 블루프린트 변수
	// 총알을 bulletCount 수만큼 동시에 발사한다.

	for (int32 i = 0; i < bulletCount; i++)
	{
		// 총알의 전체 간격
		float totalSize = (bulletCount - 1) * bulletSpacing;

		// 기준 위치
		float base_y = totalSize * -0.5f;

		// 기준 위치로 오프셋 벡터를 만든다.
		FVector offset = FVector(0, base_y + 150 * i, 0);

		FVector spawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
		// 오프셋만큼 위치를 보정한다.
		spawnPosition += offset;
		FRotator spawnRotation = FRotator(90.0f, 0, 0);
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, spawnPosition, spawnRotation, param);
		
		//생성된 총알(bullet)을 bulletAngle 만큼 일정하게 회전시킨다.
		float base_yaw = (bulletCount - 1) * bulletAngle * -0.5f;
		FRotator rot_base = FRotator(0, base_yaw + bulletAngle * i, 0);

		bullet->AddActorLocalRotation(rot_base);

	}
	// 총알 발사 효과음을 실행한다.
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

// 궁극기 폭탄 함수
void APlayerFlight::ExplosionAll()
{
	// 모든 Enemy를 파괴한다.

	// 1. TActorIterator<T>를 이용한 방식
	/*for (TActorIterator<AEnemy> enemy(GetWorld()); enemy; ++enemy)
	{
		enemy->DestroyMySelf();
	}*/

	// 2. TArray<T> 배열을 이용한 방식
	AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	TArray<AEnemy*> testArr = gm->enemies;

	if (gm != nullptr)
	{
		for (int32 i = 0; i < testArr.Num(); i++)
		{
			// Pending Kill 상태 체크
			if (IsValid(testArr[i]))
			{
				testArr[i]->DestroyMySelf();
			}
		}

		// 리스트를 한 번 초기화한다.
		gm->enemies.Empty();
	}
	
}
