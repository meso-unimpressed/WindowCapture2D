#pragma once

class WindowTouchManager;

class WCWindowTouchManager
{
public:
	WCWindowTouchManager();
	~WCWindowTouchManager();

	void InitTouches(int TouchCount);
	void TouchDown(int TouchID, int xPos, int yPos);
	void TouchMove(int TouchID, int xPos, int yPos);
	void TouchUp(int TouchID);
	void UpdateAllTouch();

private:
	WindowTouchManager* TouchManager;
};

