#include <Windows.h>

#include "../../common/savebitmap.h"

void print_screen(LPCWSTR wszFilepath)
{
	static const BYTE DIK_PTSCR = 0x37;

	// see also: SendInput()
	keybd_event(VK_SNAPSHOT, DIK_PTSCR, KEYEVENTF_EXTENDEDKEY, 0);
	keybd_event(VK_SNAPSHOT, DIK_PTSCR, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

	Sleep(1000); // wait for clipboard to be ready...

	OpenClipboard(NULL);

	HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	save_bitmap(hBitmap, wszFilepath);

	EmptyClipboard();
	CloseClipboard();
}

int main()
{
	print_screen(L"printscreen.png");
	return 0;
}