#include <Windows.h>
#include <stdio.h>

#include "tlscallback.h" // some linker magic in here

void NTAPI tls_callback(PVOID DllHandle, DWORD dwReason, PVOID lpReserved)
{
	if (dwReason == DLL_THREAD_ATTACH)
		printf("tls_callback() - new thread\n");
	else if (dwReason == DLL_PROCESS_ATTACH)
		printf("tls_callback() - new process\n");
}

DWORD WINAPI thread_proc(CONST LPVOID lpParam)
{
	printf("START thread_proc()\n");
	ExitThread(0);
}

int main()
{
	printf("START main()\n");
	
	HANDLE hThread = CreateThread(NULL, 0, &thread_proc, 0, 0, NULL);
	
	WaitForSingleObject(hThread, INFINITE);

	system("PAUSE");
	return 0;
}