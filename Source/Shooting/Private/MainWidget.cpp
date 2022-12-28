// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "MyShootingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UMainWidget::PrintCurrentScore()
{
	AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (myGM != nullptr)
	{
		// ���� ����(����) -> FText (���ڿ�) ���·� ��ȯ�Ѵ�.
		FText scoreText = FText::AsNumber(myGM->GetCurrentScore());

		//curScore �ؽ�Ʈ ����� ������ �����Ѵ�.
		curScore->SetText(scoreText);

		// �ְ� ����(����)�� ���ڿ� ���·� �ٲ۴�.
		FText bestScoreText = FText::AsNumber(myGM->GetBestScore());

		//bestScore �ؽ�Ʈ ����� ������ �����Ѵ�.
		bestScore->SetText(bestScoreText);
	}
}
