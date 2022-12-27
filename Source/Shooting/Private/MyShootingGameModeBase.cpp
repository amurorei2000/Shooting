// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShootingGameModeBase.h"
#include "MainWidget.h"

void AMyShootingGameModeBase::BeginPlay()
{
	// ���� ��������Ʈ�� �����Ѵ�.
	UMainWidget* main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

	if (main_UI != nullptr)
	{
		// ������ ������ �� ��Ʈ�� �׸���.
		main_UI->AddToViewport();

		// ���� ������ ������ curScore �ؽ�Ʈ ���Ͽ� �ݿ��Ѵ�.
		main_UI->PrintCurrentScore();
	}
}

void AMyShootingGameModeBase::AddScore(int32 count)
{
	currentScore += count;
}