#include "stdafx.h"

_Success_(return) static BOOL  __forceinline GetLockoutThreshold(_Out_ PDWORD dwNum)
{
	PUSER_MODALS_INFO_3 lpLockout = NULL;

	if (NetUserModalsGet(NULL, 3, (LPBYTE *)&lpLockout)  != NERR_Success)
		return FALSE;

	if (lpLockout == NULL)
		return FALSE;

	*dwNum = lpLockout->usrmod3_lockout_threshold;

	NetApiBufferFree(lpLockout);

	return TRUE;
}

_Success_(return) BOOL __fastcall CheckLockouts(void)
{
	DWORD dwNum;

	if (!GetLockoutThreshold(&dwNum))
	{
		printf("Unable to determine lockout policy. Use -l to force.\n");
		return FALSE;
	}

	if (dwNum != 0)
	{
		printf("Lockout policy of %d attempts. Use -l to force.\n", dwNum);
		return FALSE;
	}

	return TRUE;
}