// Copyright 2019 ayumax. All Rights Reserved.

#include "WindowCaptureActor.h"
#include "Engine/Texture2D.h"
#include "InputTransferToWindow.h"


AWindowCaptureActor::AWindowCaptureActor()
{
	PrimaryActorTick.bCanEverTick = false;	
}

void AWindowCaptureActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CaptureMachine)
	{
		CaptureMachine->Close();
		CaptureMachine = nullptr;
	}
	 
	Super::EndPlay(EndPlayReason);
}

UTexture2D* AWindowCaptureActor::Start()
{
	if (CaptureMachine)
	{
		CaptureMachine->Close();
	}

	CaptureMachine = NewObject<UCaptureMachine>(this);

	CaptureMachine->Properties = Properties;

	CaptureMachine->ChangeTexture.AddDynamic(this, &AWindowCaptureActor::OnChangeTexture);

	if (Properties.TransferMouse)
	{
		InputTransfer = NewObject<UInputTransferToWindow>(this);
		InputTransfer->ConnectingCaptureMachine(CaptureMachine);
	}

	CaptureMachine->Start();

	return CaptureMachine->CreateTexture();
}

void AWindowCaptureActor::OnChangeTexture(UTexture2D* _NewTexture)
{
	ChangeTexture.Broadcast(_NewTexture);
}

void AWindowCaptureActor::NotifyTouchOn(FVector2D UV)
{
	if (!CaptureMachine->GetTargetWindow()) return;

	if (!IsTouched && InputTransfer)
	{
		InputTransfer->NotifyMouseEvent(GetScreenPosition(UV), EMouseInputTransferType::MouseClick);
		LastTouchUV = UV;
		IsTouched = true;
	}
}

void AWindowCaptureActor::NotifyTouchEnd()
{
	if (!CaptureMachine->GetTargetWindow()) return;

	if (IsTouched && InputTransfer)
	{
		//InputTransfer->NotifyMouseEvent(GetScreenPosition(LastTouchUV), EMouseInputTransferType::MouseUp);
		IsTouched = false;
	}
}

FIntVector2D AWindowCaptureActor::GetScreenPosition(FVector2D UV)
{
	auto targetWindowSize = CaptureMachine->GetTargetWindowSize();

	POINT pt = {
		targetWindowSize.X - UV.X * targetWindowSize.X,
		targetWindowSize.Y - UV.Y * targetWindowSize.Y };

	::ClientToScreen(CaptureMachine->GetTargetWindow(), &pt);

	return FIntVector2D(pt.x, pt.y);
}
