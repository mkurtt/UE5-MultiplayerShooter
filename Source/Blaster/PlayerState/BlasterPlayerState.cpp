// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"


void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	Character = Character ? Character : Cast<ABlasterCharacter>(GetPawn());
	if (Character)
	{
		Controller = Controller ? Controller : Cast<ABlasterPlayerController>(Character->GetController());
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerState, Defeats);
}

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character ? Character : Cast<ABlasterCharacter>(GetPawn());
	if (Character)
	{
		Controller = Controller ? Controller : Cast<ABlasterPlayerController>(Character->GetController());
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}


void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character ? Character : Cast<ABlasterCharacter>(GetPawn());
	if (Character)
	{
		Controller = Controller ? Controller : Cast<ABlasterPlayerController>(Character->GetController());
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void ABlasterPlayerState::OnRep_Defeats()
{
	Character = Character ? Character : Cast<ABlasterCharacter>(GetPawn());
	if (Character)
	{
		Controller = Controller ? Controller : Cast<ABlasterPlayerController>(Character->GetController());
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}
