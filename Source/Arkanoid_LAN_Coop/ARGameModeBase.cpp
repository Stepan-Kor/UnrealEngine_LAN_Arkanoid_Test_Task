#include "ARGameModeBase.h"
// Fill out your copyright notice in the Description page of Project Settings.


//#include "ARGameModeBase.h


void AARGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(WidgetClass_MainMenu))
		Widget_MainMenu = CreateWidget<UUserWidget>(GetWorld(), WidgetClass_MainMenu);
	if (Widget_MainMenu)Widget_MainMenu->AddToViewport();
	
}
