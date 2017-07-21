#pragma once

#include <Windows.h>

static BOOL install_service(const char *szService, const char *szPath)
{
	BOOL ret = FALSE;

	SC_HANDLE hManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!hManager)
		return FALSE;

	SC_HANDLE hService = CreateServiceA(hManager, szService, szService,
		SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		szPath, NULL, NULL, NULL, NULL, NULL);

	if (hService)
	{
		ret = StartServiceA(hService, 0, NULL);
		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hManager);

	return ret;
}

static BOOL remove_service(const char *szService)
{
	BOOL ret = FALSE;

	SERVICE_STATUS_PROCESS ssp;
	DWORD dwBytesNeeded;
	SC_HANDLE hManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!hManager)
		return FALSE;

	SC_HANDLE hService = OpenServiceA(hManager, szService, SERVICE_ALL_ACCESS);
	if (hService)
	{
		if (QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssp, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
		{
			ControlService(hService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ssp);
			ret = DeleteService(hService);
			CloseServiceHandle(hService);
		}
	}


	CloseServiceHandle(hManager);

	return ret;
}