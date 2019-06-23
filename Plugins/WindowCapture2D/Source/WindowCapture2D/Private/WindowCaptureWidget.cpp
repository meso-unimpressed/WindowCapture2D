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
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!InputTransfer) return FReply::Unhandled();

	auto mousePosition = InMouseEvent.GetScreenSpacePosition();

	//InputTransfer->NotifyMouseEvent(FIntVector2D(mousePosition.X, mousePosition.Y), EMouseInputTransferType::MouseDown);

	return FReply::Unhandled();
}

FReply UWindowCaptureWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (!InputTransfer) return FReply::Unhandled();

	auto mousePosition = InMouseEvent.GetScreenSpacePosition();

	InputTransfer->NotifyMouseEvent(FIntVector2D(mousePosition.X, mousePosition.Y), EMouseInputTransferType::MouseClick);

	return FReply::Unhandled();

}