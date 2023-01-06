// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingGameModeBase.h"

APlayerBullet::APlayerBullet()
{
	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));
}

void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	//SetDirection(GetActorForwardVector());
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerBullet::OnOverlap);
}

void APlayerBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ����, �ε��� ����� ���̶��...
	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	if (enemy != nullptr)
	{
		// ���� �ִ� ��ġ�� ���� ����Ʈ�� �����Ѵ�.
		FVector enemyLoc = enemy->GetActorLocation();
		FRotator enemyRot = enemy->GetActorRotation();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_effect, enemyLoc, enemyRot, true);

		// ���� �����Ѵ�.
		enemy->Destroy();

		// ���� ����� ������ 1�� �߰��Ѵ�.
		AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
		//AGameModeBase* gm = GetWorld()->GetAuthGameMode();

		AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
		myGM->AddScore(1);
		//UE_LOG(LogTemp, Warning, TEXT("Point: %d"), myGM->GetCurrentScore());

		// �� �ڽŵ� �����Ѵ�.
		Destroy();
	}
}