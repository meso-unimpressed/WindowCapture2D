// Copyright 2019 ayumax. All Rights Reserved.

#include "WindowCaptureActor.h"
#include "Engine/Texture2D.h"
#include "WCWindowTouchManager.h"


AWindowCaptureActor::AWindowCaptureActor()
{
	PrimaryActorTick.bCanEverTick = false;	
	TouchManager = nullptr;
}

void AWindowCaptureActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CaptureMachine)
	{
		CaptureMachine->Close();
		CaptureMachine = nullptr;
	}
	 
	if (TouchManager)
	{
		delete TouchManager;
		TouchManager = nullptr;
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
		TouchManager = new WCWindowTouchManager();
		TouchManager->InitTouches(1);
	}

	CaptureMachine->Start();

	return CaptureMachine->CreateTexture();
}

void AWindowCaptureActor::OnChangeTexture(UTexture2D* _NewTexture)
{
	ChangeTexture.Broadcast(_NewTexture);
}

void AWindowCaptureActor::NotifyTouchOn(FVector2D UV, int32 Index)
{
	if (!CaptureMachine->GetTargetWindow()) return;

	if (!IsTouched && TouchManager)
	{
		HWND hWnd = CaptureMachine->GetTargetWindow();
		if (!hWnd) return;

		::BringWindowToTop(hWnd);

		auto screenPosition = GetScreenPosition(UV);
		TouchManager->TouchDown(0, screenPosition.X, screenPosition.Y);
		TouchManager->UpdateAllTouch();
		LastTouchUV = UV;
		IsTouched = true;
	}
}
void AWindowCaptureActor::NotifyTouchMove(FVector2D UV, int32 Index)
{
	if (!CaptureMachine->GetTargetWindow()) return;

	if (IsTouched && TouchManager)
	{
		HWND hWnd = CaptureMachine->GetTargetWindow();
		if (!hWnd) return;

		::BringWindowToTop(hWnd);

		auto screenPosition = GetScreenPosition(UV);
		TouchManager->TouchMove(0, screenPosition.X, screenPosition.Y);
		TouchManager->UpdateAllTouch();
		LastTouchUV = UV;
		IsTouched = true;
	}
}

void AWindowCaptureActor::NotifyTouchEnd(int32 Index)
{
	if (!CaptureMachine->GetTargetWindow()) return;

	if (IsTouched && TouchManager)
	{
		TouchManager->TouchUp(0);
		TouchManager->UpdateAllTouch();
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
