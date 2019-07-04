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
		POINT beforeMousePt;
		::GetCursorPos(&beforeMousePt);

		INPUT data[3];
		memset(data, 0, sizeof(data));
		data[0].type = INPUT_MOUSE;
		data[0].mi.dx = MOUSE_LOCATION_X(ScreenPosition.X);
		data[0].mi.dy = MOUSE_LOCATION_Y(ScreenPosition.Y);
		data[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

		data[1].type = INPUT_MOUSE;
		data[1].mi.dx = 0;
		data[1].mi.dy = 0;
		data[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		data[2].type = INPUT_MOUSE;
		data[2].mi.dx = 0;
		data[2].mi.dy = 0;
		data[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	/*	data[3].type = INPUT_MOUSE;
		data[3].mi.dx = MOUSE_LOCATION_X(beforeMousePt.x);
		data[3].mi.dy = MOUSE_LOCATION_Y(beforeMousePt.y);
		data[3].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;*/

		::SendInput(3, data, sizeof(INPUT));
	}
	else
	{
		INPUT data[4];
		memset(data, 0, sizeof(data));
		data[0].type = INPUT_MOUSE;
		data[0].mi.dx = MOUSE_LOCATION_X(ScreenPosition.X);
		data[0].mi.dy = MOUSE_LOCATION_Y(ScreenPosition.Y);
		data[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

		data[1].type = INPUT_MOUSE;
		data[1].mi.dx = 0;
		data[1].mi.dy = 0;
		data[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		switch (MouseType)
		{
		case EMouseInputTransferType::MouseDown:
			data[1].type = MOUSEEVENTF_LEFTDOWN;
			break;
		case EMouseInputTransferType::MouseUp:
			data[1].type = MOUSEEVENTF_LEFTUP;
			break;
		}

		::SendInput(2, data, sizeof(INPUT));
	}



#endif
}