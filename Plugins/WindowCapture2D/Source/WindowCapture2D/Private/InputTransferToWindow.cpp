// Copyright 2019 ayumax. All Rights Reserved.
#include "InputTransferToWindow.h"
#include "CaptureMachine.h"

UInputTransferToWindow::UInputTransferToWindow()
{

}

void UInputTransferToWindow::ConnectingCaptureMachine(UCaptureMachine* _CaptureMachine)
{
	this->CaptureMachine = _CaptureMachine;
}

//スクリーン座標->32bit座標
#define MOUSE_LOCATION_X(x) ((DWORD)x * WORD(-1)/::GetSystemMetrics(SM_CXSCREEN))
#define MOUSE_LOCATION_Y(y) ((DWORD)y * WORD(-1)/::GetSystemMetrics(SM_CYSCREEN))

void UInputTransferToWindow::NotifyMouseEvent(FIntVector2D ScreenPosition, EMouseInputTransferType MouseType)
{
	if (!CaptureMachine) return;

#if PLATFORM_WINDOWS
	HWND hWnd = CaptureMachine->GetTargetWindow();
	if (!hWnd) return;

	::BringWindowToTop(hWnd);

	if (MouseType == EMouseInputTransferType::MouseClick)
	{
		INPUT data[3];
		memset(&data, 0, sizeof(data));
		data[0].type = INPUT_MOUSE;
		data[0].mi.dx = MOUSE_LOCATION_X(ScreenPosition.X);
		data[0].mi.dy = MOUSE_LOCATION_Y(ScreenPosition.Y);
		data[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;


		::SendInput(1, data, sizeof(INPUT));
	}
	else
	{
		INPUT data;
		memset(&data, 0, sizeof(data));
		data.type = INPUT_MOUSE;
		data.mi.dx = MOUSE_LOCATION_X(ScreenPosition.X);
		data.mi.dy = MOUSE_LOCATION_Y(ScreenPosition.Y);
		data.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

		switch (MouseType)
		{
		case EMouseInputTransferType::MouseDown:
			data.type |= MOUSEEVENTF_LEFTDOWN;
			break;
		case EMouseInputTransferType::MouseUp:
			data.type |= MOUSEEVENTF_LEFTUP;
			break;
		}

		::SendInput(1, &data, sizeof(INPUT));
	}



#endif
}