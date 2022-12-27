// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShootingGameModeBase.h"
#include "MainWidget.h"

void AMyShootingGameModeBase::BeginPlay()
{
	// 위젯 블루프린트를 생성한다.
	UMainWidget* main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

	if (main_UI != nullptr)
	{
		// 생성된 위젯을 뷰 포트에 그린다.
		main_UI->AddToViewport();

		// 현재 점수를 위젯의 curScore 텍스트 블록에 반영한다.
		main_UI->PrintCurrentScore();
	}
}

void AMyShootingGameModeBase::AddScore(int32 count)
{
	currentScore += count;
}
