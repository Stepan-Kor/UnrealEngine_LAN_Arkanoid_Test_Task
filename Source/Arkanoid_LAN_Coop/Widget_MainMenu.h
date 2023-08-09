// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "ARGameInstance.h"
#include "Components/EditableTextBox.h"
#include "Widget_SessionDescription.h"
#include "Widget_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class ARKANOID_LAN_COOP_API UWidget_MainMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))UButton* ButtonExit;
	UPROPERTY(meta = (BindWidget))UButton* ButtonCreateSession;
	UPROPERTY(meta = (BindWidget))UButton* ButtonRefreshSessionList;
	UPROPERTY(meta = (BindWidget)) UEditableTextBox* EditableTextBoxSessionName;
	UPROPERTY(meta = (BindWidget)) UScrollBox* ScrollBoxSessions;
	UFUNCTION() void QuitGame();
	UFUNCTION() void RefreshSessionList();
	UFUNCTION() void RefreshSessionListComplete(UARGameInstance* GameInstanceIn);
	UFUNCTION() void CreateSession();
	UARGameInstance* GameInstance;
	UPROPERTY(EditDefaultsOnly) TSubclassOf
		<UWidget_SessionDescription> SessionDescrClass =
		UWidget_SessionDescription::StaticClass();
	APlayerController* PlayerController;
public:
	virtual bool Initialize() override;
	
};
