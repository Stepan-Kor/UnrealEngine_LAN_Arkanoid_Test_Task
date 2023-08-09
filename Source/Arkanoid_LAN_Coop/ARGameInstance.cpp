// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameInstance.h"

void UARGameInstance::Init()
{
	Super::Init();
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UARGameInstance::CreateServComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UARGameInstance::FindServComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UARGameInstance::JoinServComplete);
		}
	}
}

bool UARGameInstance::CreateServ(FName SessionName, FName HostName)
{
	if (bCreatingServ || !SessionInterface.IsValid())return false;
	bCreatingServ = true;
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	const FName TempHostName = HostName == "" ? "Empty Host Name" : HostName;
	const FName TempSessionName = SessionName == "" ? "Empty Session Name" : SessionName;
	SessionSettings.Set(FName("SERVER_NAME_KEY"), TempSessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), TempHostName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, TempSessionName, SessionSettings);
	return true;
}

bool UARGameInstance::FindServs()
{
	if (bSearchingForServs || !SessionInterface.IsValid())	return false;
	UE_LOG(LogTemp, Warning, TEXT("Game instance: Searching for sessions started."));
	bSearchingForServs = true;
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	//SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	return true;
}
void UARGameInstance::FindServComplete(bool Succesfull)
{
	bSearchingForServs = false;
	if (Succesfull)	OnServSearchFinishedWarnList.Broadcast(this);
}


bool UARGameInstance::JoinServ(FName SessionName, FOnlineSessionSearchResult& DesiredSession)
{
	UE_LOG(LogTemp, Warning, TEXT("Game instance: Joining server %s"), *SessionName.ToString());

	if (!SessionInterface.IsValid() || SessionName == "" || bJoiningServ) { 
		UE_LOG(LogTemp, Warning,	TEXT("Game instance: cant join server."));
		return false; }
	bJoiningServ = true;
	SessionInterface->JoinSession(1, SessionName, DesiredSession);
	return true;
}

void UARGameInstance::CreateServComplete(FName SessionName, bool Succesfull)
{
	bCreatingServ = false;
	if (!Succesfull) return;
	UE_LOG(LogTemp, Warning, TEXT("Game instance: Creation of session succeded %s"), *SessionName.ToString());
	/// Script / Engine.World'/Game/Level_Play.Level_Play'
	GetWorld()->ServerTravel("/Game/Level_Play?Listen");
	//GetWorld()->ServerTravel("/Game/FirstPersonCPP/Maps/FirstPersonExampleMap?Listen");
}

void UARGameInstance::JoinServComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType)
{

	bJoiningServ = false;
	if (ResultType != EOnJoinSessionCompleteResult::AlreadyInSession &&
		ResultType != EOnJoinSessionCompleteResult::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to join session."));
		return;
	}
	APlayerController* TempPController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(TempPController)) { UE_LOG(LogTemp, Warning, TEXT("GameInstance: failed to travel to new level, no controller was found.")) };
	FString TravelAdress = "";
	if (!SessionInterface.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to join session - no sessions interface."));
		return;
	};
	SessionInterface->GetResolvedConnectString(SessionName, TravelAdress);
	if (TravelAdress == "") {
		UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to join session - no adress was resolved."));
		return;
	}
	TempPController->ClientTravel(TravelAdress, ETravelType::TRAVEL_Absolute);
}
