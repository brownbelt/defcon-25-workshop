#include <Windows.h>

#include "../../common/savebitmap.h"

void take_screenshot(LPCWSTR wszFilepath)
{
	int x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN) * 2; // dirty hack
	int y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN) * 2;

	HDC     hScreen = GetDC(NULL);
	HDC     hDC = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, x2, y2);
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);

	BitBlt(hDC, 0, 0, x2, y2, hScreen, x1, y1, SRCCOPY);

	save_bitmap(hBitmap, wszFilepath);

	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
}

int main()
{
	take_screenshot(L"screenshot.png");
	return 0;
}