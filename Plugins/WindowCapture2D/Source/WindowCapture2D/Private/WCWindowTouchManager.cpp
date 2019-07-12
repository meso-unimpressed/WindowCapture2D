#include "WCWindowTouchManager.h"
#include "WindowTouchManager.h"

WCWindowTouchManager::WCWindowTouchManager()
{
	TouchManager = new WindowTouchManager();
}

WCWindowTouchManager::~WCWindowTouchManager()
{
	if (TouchManager)
	{
		delete TouchManager;
		TouchManager = nullptr;
	}
}

void WCWindowTouchManager::InitTouches(int TouchCount)
{
	TouchManager->InitTouches(TouchCount);
}

void WCWindowTouchManager::TouchDown(int TouchID, int xPos, int yPos)
{
	TouchManager->TouchDown(TouchID, xPos, yPos);
}

void WCWindowTouchManager::TouchMove(int TouchID, int xPos, int yPos)
{
	TouchManager->TouchMove(TouchID, xPos, yPos);
}

void WCWindowTouchManager::TouchUp(int TouchID)
{
	TouchManager->TouchUp(TouchID);
}

void WCWindowTouchManager::UpdateAllTouch()
{
	TouchManager->UpdateAllTouch();
}