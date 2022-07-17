// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;
	AController* InstigatorController = OwnerPawn->GetController();
	
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = Start + (HitTarget - Start) * 1.25f;

		FHitResult FireHit;
		if (GetWorld())
		{
			GetWorld()->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);

			FVector BeamEnd = End;
			if (FireHit.bBlockingHit)
			{
				BeamEnd = FireHit.ImpactPoint;
				ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
				if (BlasterCharacter)
				{
					if (HasAuthority() && InstigatorController)
					{
						UGameplayStatics::ApplyDamage(
							BlasterCharacter,
							Damage,
							InstigatorController,
							this,
							UDamageType::StaticClass()
						);
					}
				}

				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(
						this,
						ImpactParticles,
						FireHit.ImpactPoint,
						FireHit.ImpactNormal.Rotation()
					);
				}
			}
			if (BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					BeamParticles,
					SocketTransform
					);
				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEnd);
				}
			}
		}
	}
}
