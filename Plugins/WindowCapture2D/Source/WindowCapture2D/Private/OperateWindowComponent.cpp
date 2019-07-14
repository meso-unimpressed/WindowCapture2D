// Copyright 2019 ayumax. All Rights Reserved.
#include "OperateWindowComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "WindowCaptureActor.h"
#include "GameFramework/Actor.h"

UOperateWindowComponent::UOperateWindowComponent()
{
	SetComponentTickEnabled(true);

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UOperateWindowComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OwnerComponent) return;

	FHitResult OutHit;

	auto ownerLocation = OwnerComponent->GetComponentLocation();
	auto ownerRotaion = OwnerComponent->GetComponentRotation();

	FVector TraceEnd = ownerLocation + ownerRotaion.Vector() * 2000.0f;

	FCollisionQueryParams CollisionParams;

	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnFaceIndex = true;

	//DrawDebugLine(GetWorld(), ownerLocation, TraceEnd, FColor::Green, false, 1, 0, 1);

	bool isTouched = false;

	if (IsEnable)
	{
		if (GetWorld()->LineTraceSingleByChannel(OutHit, ownerLocation, TraceEnd, ECC_Visibility, CollisionParams))
		{
			if (OutHit.bBlockingHit)
			{
				bool isTouchedOn = TouchedActor != nullptr;

				TouchedActor = Cast<AWindowCaptureActor>(OutHit.GetActor());
				if (TouchedActor)
				{
					FVector2D uv;
					if (UGameplayStatics::FindCollisionUV(OutHit, 0, uv))
					{
						if (isTouchedOn)
						{
							TouchedActor->NotifyTouchMove(uv, TouchIndex);
						}
						else
						{
							TouchedActor->NotifyTouchOn(uv, TouchIndex);
						}

						isTouched = true;
					}
				}
			}
		}
	}
	
	
	if (TouchedActor && !isTouched)
	{
		TouchedActor->NotifyTouchEnd(TouchIndex);
		TouchedActor = nullptr;
	}
}

void UOperateWindowComponent::SetOwnerComponent(USceneComponent* _OwnerComponent)
{
	OwnerComponent = _OwnerComponent;
}

void UOperateWindowComponent::SetEnable(bool _IsEnable, int32 _TouchIndex)
{
	IsEnable = _IsEnable;
	TouchIndex = _TouchIndex;
}