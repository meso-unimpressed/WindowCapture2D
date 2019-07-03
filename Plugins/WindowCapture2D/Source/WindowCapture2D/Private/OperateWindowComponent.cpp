// Copyright 2019 ayumax. All Rights Reserved.
#include "OperateWindowComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

UOperateWindowComponent::UOperateWindowComponent()
{
	SetComponentTickEnabled(true);

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UOperateWindowComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult OutHit;

	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	FVector TraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * 100.0f;

	FCollisionQueryParams CollisionParams;

	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnFaceIndex = true;

	DrawDebugLine(GetWorld(), PlayerViewLocation, TraceEnd, FColor::Green, false, 1, 0, 1);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, PlayerViewLocation, TraceEnd, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			if (GEngine) 
			{
				FVector2D uv;
				UGameplayStatics::FindCollisionUV(OutHit, 0, uv);
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Impact Point: %f, %f"), uv.X, uv.Y));

				}

			}
		}
	}
}