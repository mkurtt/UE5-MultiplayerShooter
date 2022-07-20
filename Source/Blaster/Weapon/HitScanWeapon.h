// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

protected:
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget);
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

public:
	virtual void Fire(const FVector& HitTarget) override;

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* BeamParticles;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	class USoundCue* FireSound;
	UPROPERTY(EditAnywhere)
	class USoundCue* HitSound;

	//trace end with scatter

	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	float DistanceToSphere = 800.f;
	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	float SphereRadius = 75.f;
	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	bool bUseScatter = false;
};
