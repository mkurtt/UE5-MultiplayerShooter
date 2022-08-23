// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldPickup.h"

#include "Blaster/BlasterComponents/BuffComponent.h"
#include "Blaster/Character/BlasterCharacter.h"

void AShieldPickup::OnSphereOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(PrimitiveComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp,Warning,TEXT("OnSphereOverlap  1"));
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		UE_LOG(LogTemp,Warning,TEXT("OnSphereOverlap  2"));
		UBuffComponent* Buff = BlasterCharacter->GetBuff();
		if (Buff)
		{
			UE_LOG(LogTemp,Warning,TEXT("OnSphereOverlap  3"));
			Buff->ReplenishShield(ReplenishAmount,ReplenishTime);
		}
	}
	Destroy();
}
