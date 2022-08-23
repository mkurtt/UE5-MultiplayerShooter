#include "PickupSpawnPoint.h"

#include "Pickup.h"

APickupSpawnPoint::APickupSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnPickupTimer(nullptr);
}

void APickupSpawnPoint::SpawnPickup()
{
	int32 NumPickupClasses = PickupClasses.Num();
	if (NumPickupClasses <= 0) return;

	int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);
	SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection], GetActorTransform());

	if(HasAuthority() && SpawnedPickup)
	{
		SpawnedPickup->OnDestroyed.AddDynamic(this, &ThisClass::StartSpawnPickupTimer);
	}
}

void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	GetWorldTimerManager().SetTimer(SpawnPickupTimer, this, &ThisClass::OnSpawnPickupTimerFinished, FMath::RandRange(SpawnTimeMin, SpawnTimeMax));
}

void APickupSpawnPoint::OnSpawnPickupTimerFinished()
{
	if (HasAuthority())
		SpawnPickup();
}

void APickupSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
