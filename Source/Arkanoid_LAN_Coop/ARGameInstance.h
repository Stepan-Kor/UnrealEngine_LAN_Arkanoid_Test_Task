// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ARGameInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FServSearchFinishedDelegateType, UARGameInstance*);
UCLASS()
class ARKANOID_LAN_COOP_API UARGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	bool bSearchingForServs = false;
	bool bCreatingServ = false;
	bool bJoiningServ = false;
public:
	IOnlineSessionPtr SessionInterface;
	bool CreateServ(FName SessionName, FName HostName);
	bool FindServs();
	bool JoinServ(FName SessionName, FOnlineSessionSearchResult& DesiredSession);
	void CreateServComplete(FName SessionName, bool Succesfull);
	void FindServComplete(bool Succesfull);
	void JoinServComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType);
	virtual void Init() override;
	FServSearchFinishedDelegateType OnServSearchFinishedWarnList;
	TSharedPtr <FOnlineSessionSearch> SessionSearch;
};
