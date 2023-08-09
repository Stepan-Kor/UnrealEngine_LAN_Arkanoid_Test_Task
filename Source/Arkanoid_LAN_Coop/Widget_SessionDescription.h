// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OnlineSessionSettings.h"
#include "Widget_SessionDescription.generated.h"

/**
 * 
 */
UCLASS()
class ARKANOID_LAN_COOP_API UWidget_SessionDescription : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))UButton* Button_Join;
	UPROPERTY(meta = (BindWidget))UTextBlock* TextBlock_ServerName;
	UFUNCTION() void JoinServer();
	FName ServerName{"EmptyName"};
	FOnlineSessionSearchResult DesiredServer;
public:
	virtual bool Initialize() override;
	void StartSetting(FOnlineSessionSearchResult DesiredServ,FName ServName="");

};
