#include "move_logic.h"
int xPos;
int yPos;
int xDirection;
int yDirection;
bool isAutoMoving;
void MoveSprite(HWND hwnd, int offsetX, int offsetY) {
	xPos += offsetX;
	yPos += offsetY;
	InvalidateRect(hwnd, NULL, FALSE);
}
void SetSpriteX(int x) {
	xPos = x;
}
void SetSpriteY(int y) {
	yPos = y;
}
void SetSpriteXDirection() {
	xDirection = ~xDirection + 1;
}void SetSpriteYDirection() {
	yDirection = ~yDirection + 1;
}
void ChangeTimers(HWND hwnd)
{
	if (isAutoMoving) {
		isAutoMoving = false;
		KillTimer(hwnd, IDT_TIMER2);
		SetTimer(hwnd, IDT_TIMER1, 2000, (TIMERPROC)NULL);
	}
	else {
		KillTimer(hwnd, IDT_TIMER1);
		SetTimer(hwnd, IDT_TIMER1, 2000, (TIMERPROC)NULL);
	}
}
void CheckWindowBorders(RECT windowRect) {
	if (windowRect.left >= xPos) {
		if (isAutoMoving)
			SetSpriteXDirection();
		SetSpriteX(windowRect.left);
	}
	if (windowRect.right <= xPos)
	{
		if (isAutoMoving)
			SetSpriteXDirection();
		SetSpriteX(windowRect.right);
	}
	if (windowRect.bottom <= yPos) {
		if (isAutoMoving)
			SetSpriteYDirection();
		SetSpriteY(windowRect.bottom);
	}
	if (windowRect.top >= yPos) {
		if (isAutoMoving)
			SetSpriteYDirection();
		SetSpriteY(windowRect.top);
	}
}