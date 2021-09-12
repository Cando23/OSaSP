#pragma once
#include <windows.h>
#define IDT_TIMER1 1
#define IDT_TIMER2 2
extern int xPos;
extern int yPos;
extern int xDirection;
extern int yDirection;
extern bool isAutoMoving;
void MoveSprite(HWND hwnd, int offsetX, int offsetY);
void SetSpriteX(int x);
void SetSpriteY(int y);
void SetSpriteXDirection();
void SetSpriteYDirection();
void ChangeTimers(HWND hwnd);
void CheckWindowBorders(RECT windowRect);