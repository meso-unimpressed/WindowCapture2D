// Copyright 2019 ayumax. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CaptureMachineProperties.h"
#include "InputTransferToWindow.generated.h"

class UCaptureMachine;

UCLASS(BlueprintType, Blueprintable)
class WINDOWCAPTURE2D_API UInputTransferToWindow : public UObject
{
	GENERATED_BODY()

public:
	UInputTransferToWindow();

	void ConnectingCaptureMachine(UCaptureMachine* CaptureMachine);

	UFUNCTION(BlueprintCallable, Category = WindowCapture2D)
	void NotifyMouseEvent(FIntVector2D ScreenPosition, EMouseInputTransferType MouseType);

private:
	UPROPERTY(Transient)
	UCaptureMachine* CaptureMachine;
};