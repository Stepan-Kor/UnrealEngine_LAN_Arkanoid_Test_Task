// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SessionDescription.h"
#include "ARGameInstance.h"

bool UWidget_SessionDescription::Initialize()
{
	Super::Initialize();
	if (Button_Join)Button_Join->OnClicked.AddDynamic(this, &UWidget_SessionDescription::JoinServer);

	return true;
}
void UWidget_SessionDescription::StartSetting
	(FOnlineSessionSearchResult DesiredServ, FName ServName)
{
	if (ServName != "")ServerName = ServName;
	DesiredServer = DesiredServ;
	if (TextBlock_ServerName)TextBlock_ServerName->SetText(FText::FromName(ServerName));
}
void UWidget_SessionDescription::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Serv descr: Joining server %s"), *ServerName.ToString());
	auto GameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!GameInstance) { 
		UE_LOG(LogTemp, Warning, TEXT("Serv descr: cant get gameinstance"));
		return; }
	bool bResult =GameInstance->JoinServ(ServerName, DesiredServer);
	if(!bResult)	UE_LOG(LogTemp, Warning, TEXT("Serv descr: cant join"), *ServerName.ToString());
}
