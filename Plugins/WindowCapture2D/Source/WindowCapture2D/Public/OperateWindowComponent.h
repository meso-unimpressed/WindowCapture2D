// Copyright 2019 ayumax. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "OperateWindowComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class WINDOWCAPTURE2D_API UOperateWindowComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOperateWindowComponent();

	UFUNCTION(BlueprintCallable, Category = WindowCapture2D)
	void SetOwnerComponent(USceneComponent* OwnerComponent);

	UFUNCTION(BlueprintCallable, Category = WindowCapture2D)
	void SetEnable(bool IsEnable);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(Transient)
	class AWindowCaptureActor* TouchedActor = nullptr;

	UPROPERTY(Transient)
	USceneComponent* OwnerComponent = nullptr;

	bool IsEnable;
};