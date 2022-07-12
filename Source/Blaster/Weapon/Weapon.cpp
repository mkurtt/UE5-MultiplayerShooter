// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Casing.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;


	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
	}

	if (PickupWidget)
		PickupWidget->SetVisibility(false);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		BlasterCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		BlasterCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	switch (WeaponState)
	{
		case EWeaponState::EWS_Initial: break;
		case EWeaponState::EWS_Equipped: ShowPickupWidget(false);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EWeaponState::EWS_Dropped: break;
		case EWeaponState::EWS_MAX: break;
		default: ;
	}
}

void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
		case EWeaponState::EWS_Initial: break;
		case EWeaponState::EWS_Equipped: ShowPickupWidget(false);
			break;
		case EWeaponState::EWS_Dropped: break;
		case EWeaponState::EWS_MAX: break;
		default: ;
	}
}


void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
		PickupWidget->SetVisibility(bShowWidget);
}

void AWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
		if (CasingClass)
		{
			const USkeletalMeshSocket* AmmoEjectSocket = GetWeaponMesh()->GetSocketByName(FName("AmmoEject"));
			if (AmmoEjectSocket)
			{
				FTransform AmmoEjectTransform = AmmoEjectSocket->GetSocketTransform(GetWeaponMesh());
				FActorSpawnParameters SpawnParams;

				if (GetWorld())
					GetWorld()->SpawnActor<ACasing>(CasingClass, AmmoEjectTransform.GetLocation(), AmmoEjectTransform.GetRotation().Rotator(), SpawnParams);
			}
		}
	}
}
