// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_MainMenu.h"
#include "Kismet/GameplayStatics.h"

bool UWidget_MainMenu::Initialize()
{
	Super::Initialize();
	if (ButtonExit)ButtonExit->OnClicked.AddDynamic(this, &UWidget_MainMenu::QuitGame);
	if (ButtonRefreshSessionList)ButtonRefreshSessionList->OnClicked.AddDynamic(this, &UWidget_MainMenu::RefreshSessionList);
	if (ButtonCreateSession)ButtonCreateSession->OnClicked.AddDynamic(this, &UWidget_MainMenu::CreateSession);
	GameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (GameInstance)
		GameInstance->OnServSearchFinishedWarnList.AddUObject(this,&UWidget_MainMenu::RefreshSessionListComplete);
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController) {
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
	};
	return true;
}

void UWidget_MainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),PlayerController,
		EQuitPreference::Quit,true);
}

void UWidget_MainMenu::RefreshSessionList()
{
	if (!IsValid(GameInstance) || !IsValid(ScrollBoxSessions))return;
	bool SearchStarted = GameInstance->FindServs();
	if (!SearchStarted)return;
	ButtonRefreshSessionList->SetIsEnabled(false);
	//ButtonRefreshSessionList
	ScrollBoxSessions->ClearChildren();
}

void UWidget_MainMenu::RefreshSessionListComplete(UARGameInstance* GameInstanceIn)
{
	ButtonRefreshSessionList->SetIsEnabled(true);
	if (!IsValid(GameInstance) || !IsValid(ScrollBoxSessions)) {
		UE_LOG(LogTemp, Warning, TEXT("Widget Main menu : cant sort search server results."));
		return; }

	UE_LOG(LogTemp, Warning, TEXT("Widget Main menu : Got search server results."));
	UWidget_SessionDescription* TempWidget;
	int8 Counter{ 0 };
	FString TempString{};
	for (FOnlineSessionSearchResult Result : GameInstance->SessionSearch->SearchResults) {
		if (Counter > 1000) break;
		TempString= "Session Description " ;
		TempString.Append(FString::FromInt(Counter));
		UE_LOG(LogTemp, Warning, TEXT("Widget Main menu : Got server %s."),*TempString);
		Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), TempString);
		TempWidget = CreateWidget<UWidget_SessionDescription>(this, SessionDescrClass,FName(TempString));
		if (IsValid(TempWidget)) {
			TempWidget->StartSetting(Result, FName(TempString));
			ScrollBoxSessions->AddChild(TempWidget);
		}
		Counter++;
	};
}

void UWidget_MainMenu::CreateSession()
{
	if (!GameInstance || !EditableTextBoxSessionName) return;
	FName TempSessionName=
		FName(EditableTextBoxSessionName->GetText().ToString());
	if (TempSessionName == "") TempSessionName = "Empty Session Name";
	FName HostName = FName("Host Name");
	GameInstance->CreateServ(TempSessionName, HostName);
}

