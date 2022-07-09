// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
		case ROLE_None: Role = FString("None");
			break;
		case ROLE_SimulatedProxy: Role = FString("SimulatedProxy");
			break;
		case ROLE_AutonomousProxy: Role = FString("AutonomousProxy");
			break;
		case ROLE_Authority: Role = FString("Authority");
			break;
	}
	FString RemoteRoleRoleString = FString::Printf(TEXT("RemoteRole Role: %s"), *Role);
	SetDisplayText(RemoteRoleRoleString);
}

void UOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	auto PlayerState = InPawn->GetPlayerState();
	
	if (PlayerState)
	{
		SetDisplayText(PlayerState->GetPlayerName());
	}
}

void UOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
