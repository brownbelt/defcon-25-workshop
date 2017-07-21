#include "stdafx.h"

VOID __fastcall InitCrackSet(_Inout_ PCRACK_SET lpCrackSet, _In_ PSTEAMROLL_OPTIONS options)
{
	PNET_USER pEntry;
	time_t tmT;

	InitAccountList(&lpCrackSet->lpUserList, options->szUser);
	InitDictionary(&lpCrackSet->dictionary, options->szFile);

	lpCrackSet->dwPasswordAttempts = 0;
	lpCrackSet->dwUnsolved = 0;

	time(&tmT);

	NET_USER_LOOP(pEntry, lpCrackSet->lpUserList)
	{
		if (!options->bDisabledUsers && (pEntry->dwFlags & UF_ACCOUNTDISABLE) == UF_ACCOUNTDISABLE)
		{
			pEntry->bActive = FALSE;
			printf("[-] Skipping disabled user: %ls. No way to force.\n\n", pEntry->szUser);
			continue;
		}

		if (pEntry->dwAccountExpires != TIMEQ_FOREVER && (uint32_t)pEntry->dwAccountExpires - tmT < 0)
		{
			pEntry->bActive = FALSE;
			printf("[-] Skipping expired user: %ls. No way to force.\n\n", pEntry->szUser);
			continue;
		}

		if (!options->bExpiredPasswords && (pEntry->dwFlags & UF_PASSWORD_EXPIRED) == UF_PASSWORD_EXPIRED)
		{
			pEntry->bActive = FALSE;
			printf("[-] Skipping expired password: %ls. No way to force.\n\n", pEntry->szUser);
			continue;
		}

		++lpCrackSet->dwUnsolved;
	}

	lpCrackSet->hSetMutex = CreateMutex(NULL, FALSE, NULL);
}

_Success_(return) BOOL __fastcall GetNextCrackSet(PCRACK_SET lpCrackSet, _In_opt_ LPVOID lpReserved, _Out_writes_z_(nPasswordSize) LPTSTR szPassword, _In_ int nPasswordSize)
{
	DWORD dwWaitResult;
	BOOL success = FALSE;

	szPassword[0] = '\0';

	if (lpCrackSet->dwUnsolved == 0)
		return FALSE;

	dwWaitResult = WaitForSingleObject(lpCrackSet->hSetMutex, INFINITE);

	if (dwWaitResult == WAIT_ABANDONED)
		return FALSE;

	__try
	{
		++lpCrackSet->dwPasswordAttempts;

		success = NextDictionaryWord(&lpCrackSet->dictionary, szPassword, nPasswordSize);
	}
	__finally 
	{
		ReleaseMutex(lpCrackSet->hSetMutex);
	}

	return success;
}

/* Only partially implemented */
VOID __fastcall FreeCrackSet(_In_ PCRACK_SET lpCrackSet)
{
	/*	
	FreeAccountList(lpCrackSet->lpUserList);
	FreeDictionary(lpCrackSet->dictionary);
	*/
	CloseHandle(lpCrackSet->hSetMutex);
}