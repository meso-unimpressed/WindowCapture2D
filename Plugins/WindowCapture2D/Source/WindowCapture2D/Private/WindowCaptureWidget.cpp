// Copyright 2019 ayumax. All Rights Reserved.

#include "WindowCaptureWidget.h"
#include "Engine/Texture2D.h"
#include "InputTransferToWindow.h"

UWindowCaptureWidget::UWindowCaptureWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWindowCaptureWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	if (CaptureMachine)
	{
		FlushRenderingCommands();
		CaptureMachine->Close();
		CaptureMachine = nullptr;
	}

	Super::ReleaseSlateResources(bReleaseChildren);
}

UTexture2D* UWindowCaptureWidget::Start()
{
	if (CaptureMachine)
	{
		CaptureMachine->Close();
	}

	CaptureMachine = NewObject<UCaptureMachine>(this);

	CaptureMachine->Properties = Properties;

	CaptureMachine->ChangeTexture.AddDynamic(this, &UWindowCaptureWidget::OnChangeTexture);

	if (Properties.TransferMouse)
	{
		InputTransfer = NewObject<UInputTransferToWindow>(this);
		InputTransfer->ConnectingCaptureMachine(CaptureMachine);
	}

	CaptureMachine->Start();

	return CaptureMachine->CreateTexture();
}

void UWindowCaptureWidget::OnChangeTexture(UTexture2D* _NewTexture)
{
	ChangeTexture.Broadcast(_NewTexture);
}

FReply UWindowCaptureWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto rep = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!InputTransfer) return FReply::Unhandled();

	auto mousePosition = InMouseEvent.GetScreenSpacePosition();

	//InputTransfer->NotifyMouseEvent(FIntVector2D(mousePosition.X, mousePosition.Y), EMouseInputTransferType::MouseDown);

	return rep;
}

FReply UWindowCaptureWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto rep = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (!InputTransfer) return FReply::Unhandled();

	auto mousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	auto targetWindowSize = CaptureMachine->GetTargetWindowSize();

	POINT pt = { 
		mousePosition.X / InGeometry.GetLocalSize().X * targetWindowSize.X,
		mousePosition.Y / InGeometry.GetLocalSize().Y * targetWindowSize.Y};

	::ClientToScreen(CaptureMachine->GetTargetWindow(), &pt);

	InputTransfer->NotifyMouseEvent(FIntVector2D(pt.x, pt.y), EMouseInputTransferType::MouseClick);

	return rep;

}