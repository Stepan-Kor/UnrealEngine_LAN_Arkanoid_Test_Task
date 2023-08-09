// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PlayScreen.h"
#include "Kismet/GameplayStatics.h"
#include "GameState_Play.h"
#include "ARController.h"

void UWidget_PlayScreen::PointsCatch(int Points)
{
	CurentPoints = Points;
	FString TempString{ "Points: " };
	TempString.Append(FString::FromInt(Points));
	if (TextBlock_Points)TextBlock_Points->SetText(FText::FromString(TempString));
}

bool UWidget_PlayScreen::Initialize()
{
	Super::Initialize();
	GameInstance = Cast<UARGameInstance>(GetGameInstance());
	//PlController = Cast <AARController>( GetWorld()->GetFirstPlayerController<APlayerController>());
	if (ButtonExit)ButtonExit->OnClicked.AddDynamic(this, &UWidget_PlayScreen::QuitGame);
	if (ButtonReadyForNext)ButtonReadyForNext->OnClicked.AddDynamic(this, &UWidget_PlayScreen::ReadyForNext);

	return true;
}

void UWidget_PlayScreen::GameFinished(bool Type)
{
	bGameFinished = Type;
	if (TextFinished) {
		TextFinished->SetVisibility(ESlateVisibility::Visible);
		if (Type == false) { 
			FSlateColor Color = FSlateColor(FColor::Red);
			TextFinished->SetColorAndOpacity(Color);
		}
	}
}

void UWidget_PlayScreen::StartSetting(AARController* Contr)
{
	auto GameState = Cast<AGameState_Play>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState) {
		GameState->Widget_Screen = this;
		GameState->PointsWarnList.AddUObject(this, &UWidget_PlayScreen::PointsCatch);
	}
	PlController = Contr;
	if (Contr)Contr->CallPoints();
}

AARController* UWidget_PlayScreen::GetPlayerController()
{
	//if (PlController)return PlController;
	//PlController = Cast <AARController>(GetWorld()->GetFirstPlayerController<APlayerController>());
	return PlController;
}

void UWidget_PlayScreen::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), PlController,	EQuitPreference::Quit, true);
}

void UWidget_PlayScreen::ReadyForNext()
{
	UE_LOG(LogTemp, Warning, TEXT("Widget Play screen: next ready pressed %s."), *this->GetName());
	if (!GetPlayerController()) { 
		UE_LOG(LogTemp, Warning, TEXT("Widget Play screen: cant get pl controller %s."), *this->GetName());
		return; }
	PlController->ChangeReadyForNextState(true);
	//auto GameState = Cast<AGameState_Play>(UGameplayStatics::GetGameState(GetWorld()));
	if (ButtonReadyForNext)ButtonReadyForNext->SetBackgroundColor(FColor::Green);
}
