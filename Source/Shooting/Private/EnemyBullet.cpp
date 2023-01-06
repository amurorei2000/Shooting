// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingGameModeBase.h"
#include "PlayerFlight.h"

AEnemyBullet::AEnemyBullet()
{
	boxComp->SetCollisionProfileName(TEXT("Enemy"));
}


void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlap);
}

void AEnemyBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 부딪힌 대상이 플레이어라면...
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr)
	{
		// 플레이어를 제거한다.
		player->Destroy();

		// 메뉴 위젯을 생성하는 함수를 실행한다.
		AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (gm != nullptr)
		{
			gm->ShowMenu();
		}

		Destroy();
	}
}
