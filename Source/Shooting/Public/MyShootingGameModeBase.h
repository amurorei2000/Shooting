// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyShootingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMyShootingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category=MyDefaultSetting)
	TSubclassOf<class UMainWidget> mainWidget;

	UPROPERTY(EditAnywhere, Category=MyDefaultSetting)
	TSubclassOf<class UMenuWidget> menuWidget;

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentScore() { return currentScore;}
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetBestScore() { return bestScore; }

	virtual void BeginPlay() override;

	void AddScore(int32 count);

	void ShowMenu();
	
private:
	int32 currentScore = 0;
	int32 bestScore = 0;
	class UMainWidget* main_UI;
	class UMenuWidget* menu_UI;

	FString filePath = FString("C:/Unreal Projects/Shooting/Content/SaveScore/BestScore.txt");
	FString filePath2 = FString("../../../Content/SaveScore/BestScore.txt");
};
