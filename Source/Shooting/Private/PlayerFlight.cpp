// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

APlayerFlight::APlayerFlight()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 박스 컴포넌트를 생성한다.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	// 생성된 충돌 박스 컴포넌트를 루트 컴포넌트로 설정한다.
	SetRootComponent(boxComp);
	
	// 박스 콜리전의 크기를 가로x세로x높이 모두 50센티미터로 설정한다.
	boxComp->SetBoxExtent(FVector(50.0f));

	// 메시 컴포넌트를 생성한다.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	// 메시 컴포넌트를 루트 컴포넌트의 하위 개체로 등록한다.
	meshComp->SetupAttachment(RootComponent);

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
	
}

void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Horizontal Axis 입력에 함수를 연결한다.
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);
	
	// Vertical Axis 입력에 함수를 연결한다.
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical);

}

// 좌우 입력이 있을 때 실행될 함수
void APlayerFlight::Horizontal(float value)
{
	h = value;
	UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
}

// 상하 입력이 있을 때 실행될 함수
void APlayerFlight::Vertical(float value)
{
	v = value;
	UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
}