// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 *
 */
UCLASS()
class SHOOTING_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting|Score", meta = (BindWidget))
	class UTextBlock* curScore;

	UPROPERTY(EditAnywhere, Category ="MySetting|Score", meta=(BindWidget))
	class UTextBlock* bestScore;

	void PrintCurrentScore();

};
