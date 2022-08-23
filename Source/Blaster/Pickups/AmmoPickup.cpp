// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"

#include "Blaster/BlasterComponents/CombatComponent.h"
#include "Blaster/Character/BlasterCharacter.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(PrimitiveComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		UCombatComponent* CombatComponent = BlasterCharacter->GetCombat();
		if (CombatComponent)
		{
			CombatComponent->PickupAmmo(WeaponType, AmmoAmount);
		}
	}
	Destroy();
}
