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
	// �ε��� ����� �÷��̾���...
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr)
	{
		// �÷��̾ �����Ѵ�.
		player->Destroy();

		// �޴� ������ �����ϴ� �Լ��� �����Ѵ�.
		AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (gm != nullptr)
		{
			gm->ShowMenu();
		}

		Destroy();
	}
}
