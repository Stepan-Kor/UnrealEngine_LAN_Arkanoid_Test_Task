// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ARGameInstance.h"
#include "Widget_PlayScreen.generated.h"

/**
 * 
 */
UCLASS()
class ARKANOID_LAN_COOP_API UWidget_PlayScreen : public UUserWidget
{
	GENERATED_BODY()
protected:
UPROPERTY(meta = (BindWidget))UButton* ButtonExit;
UPROPERTY(meta = (BindWidget))UButton* ButtonReadyForNext;
UPROPERTY(meta = (BindWidget))UTextBlock* TextFinished;
UPROPERTY(meta = (BindWidget))UTextBlock* TextBlock_Points;
UFUNCTION() void QuitGame();
UFUNCTION() void ReadyForNext();
UARGameInstance* GameInstance;
bool bGameFinished{ false };
public:
	int CurentPoints{0};
	UFUNCTION() void PointsCatch(int Points);
	virtual bool Initialize() override;
	void GameFinished(bool Type=true);
	class AARController* PlController;
	void StartSetting(AARController* Contr);
	AARController* GetPlayerController();

};
