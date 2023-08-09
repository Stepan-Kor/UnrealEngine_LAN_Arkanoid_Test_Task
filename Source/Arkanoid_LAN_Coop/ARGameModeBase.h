// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widget_MainMenu.h"
#include "ARGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ARKANOID_LAN_COOP_API AARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> WidgetClass_MainMenu = UWidget_MainMenu::StaticClass();

protected:
	virtual void BeginPlay() override; 
	UUserWidget* Widget_MainMenu;
};
