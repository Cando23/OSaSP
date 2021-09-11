#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <winuser.h>
#include <windowsx.h>

HBITMAP hBitmap = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Window Class";

	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Sprite",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
int xPos;
int yPos;
const int OFFSET = 20;
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
void CheckWindowBorders(RECT windowRect) {
	if (windowRect.left > xPos) {
		SetSpriteX(windowRect.left);
	}
	if (windowRect.right <= xPos)
	{
		SetSpriteX(windowRect.right);
	}
	if (windowRect.bottom <= yPos) {
		SetSpriteY(windowRect.bottom);
	}
	if (windowRect.top > yPos) {
		SetSpriteY(windowRect.top);
	}
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	RECT windowRect;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		GetWindowRect(hwnd, &windowRect);
		SetSpriteX((windowRect.right - windowRect.left) / 2);
		SetSpriteY((windowRect.bottom - windowRect.top) / 2);
		hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), L"../image.bmp", IMAGE_BITMAP, 0, 0,
			LR_DEFAULTSIZE | LR_LOADFROMFILE);
	}
	break;
	case WM_DESTROY:
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		return 0;
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BITMAP bitmap;
		HDC hdcMem;
		HGDIOBJ oldBitmap;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmap);
		GetObject(hBitmap, sizeof(bitmap), &bitmap);

		GetWindowRect(hwnd, &windowRect);
		windowRect.right = windowRect.right - bitmap.bmWidth / 2 - windowRect.left - 5;
		windowRect.left = bitmap.bmWidth / 2;
		windowRect.bottom = windowRect.bottom - windowRect.top - bitmap.bmHeight / 2 - 35;
		windowRect.top = bitmap.bmHeight / 2;
		CheckWindowBorders(windowRect);

		BitBlt(hdc, xPos - bitmap.bmWidth / 2, yPos - bitmap.bmHeight / 2, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);
	}
	break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_UP:
			MoveSprite(hwnd, 0, -OFFSET);
			break;
		case VK_DOWN:
			MoveSprite(hwnd, 0, OFFSET);
			break;
		case VK_LEFT:
			MoveSprite(hwnd, -OFFSET, 0);
			break;
		case VK_RIGHT:
			MoveSprite(hwnd, OFFSET, 0);
			break;
		}
		break;
	case WM_MOUSEMOVE:
	{
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		InvalidateRect(hwnd, NULL, FALSE);
	}
	case WM_MOUSEWHEEL: {
		int direction = GET_WHEEL_DELTA_WPARAM(wParam) < 0 ? 1 : -1;
		if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
		{
			MoveSprite(hwnd, OFFSET * direction, 0);
		}
		else
		{
			MoveSprite(hwnd, 0, OFFSET * direction);
		}
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}