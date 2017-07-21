#include <Windows.h>
#include <stdio.h>

HHOOK hHook;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT pk = (PKBDLLHOOKSTRUCT)lParam;

	printf("wParam: %s\t", (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ? "DOWN" : "UP");

	printf("scanCode: %d\t", pk->scanCode);
	printf("vkCode: %d\t", pk->vkCode);
	printf("ASCII: %c\n", pk->vkCode);

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main()
{
	MSG msg;

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}