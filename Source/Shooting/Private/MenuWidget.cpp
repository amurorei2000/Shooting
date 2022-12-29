// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"


void UMenuWidget::NativeConstruct()
{
	btn_Resume->OnClicked.AddDynamic(this, &UMenuWidget::ResumeGame);
	btn_Restart->OnClicked.AddDynamic(this, &UMenuWidget::RestartGame);
	btn_Quit->OnClicked.AddDynamic(this, &UMenuWidget::QuitGame);
}

void UMenuWidget::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	this->RemoveFromParent();
}

void UMenuWidget::RestartGame()
{
	// 레벨을 다시 로드한다.
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ShootingMap"));

	// 현재 위젯은 제거한다.
	this->RemoveFromParent();
}

void UMenuWidget::QuitGame()
{
	// 앱을 종료한다.
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), playerCon, EQuitPreference::Quit, true);
}
