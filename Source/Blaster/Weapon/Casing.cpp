// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;
	bBeingDestroyed = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);

	ShellEjectionImpulse = 10.f;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->AddImpulse(GetActorForwardVector() * FMath::RandRange(ShellEjectionImpulse - 1, ShellEjectionImpulse + 1));
	CasingMesh->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bBeingDestroyed) return;
	bBeingDestroyed = true;
	
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}
	FTimerHandle Delay;
	GetWorldTimerManager().SetTimer(Delay, [&]()
	{
		Destroy();
	}, 2, false);
}
