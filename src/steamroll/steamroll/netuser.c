#include "stdafx.h"

static VOID __forceinline AccountListTail(_Out_ PNET_USER pNetUser)
{
	pNetUser->bActive = TRUE;
	pNetUser->lpNext = NULL;
}

static VOID __fastcall InsertAccount(_Inout_ PNET_USER *pListHead, _In_ LPCTSTR szUser, _In_opt_ DWORD dwFlags, _In_opt_ DWORD dwAccountType, _In_opt_ DWORD dwPasswordAge, _In_opt_ DWORD dwAccountExpires)
{
	PNET_USER pEntry = NULL;

	if (*pListHead == NULL)
		return;

	//for (pEntry = *pListHead; pEntry->lpNext != NULL; pEntry = pEntry->lpNext)
	NET_USER_LOOP(pEntry, *pListHead) continue;

	_tcscpy_s(pEntry->szUser, NET_USER_BUFFERSIZE, szUser);
	pEntry->dwFlags = dwFlags;
	pEntry->dwAccountType = dwAccountType;
	pEntry->dwPasswordAge = dwPasswordAge;
	pEntry->dwAccountExpires = dwAccountExpires;

	/* bad alloc */
	if ((pEntry->lpNext = (PNET_USER)malloc(sizeof(NET_USER))) == NULL)
		return;

	AccountListTail(pEntry->lpNext);
}

VOID __fastcall InitAccountList(_Inout_ PNET_USER *pListHead, _In_opt_ LPCTSTR szUser)
{
	LPUSER_INFO_2 lpAccounts, lpNtBuf = NULL;
	DWORD dwEntriesRead, dwTotalEntries, i;
	
	if ((*pListHead = (PNET_USER)malloc(sizeof(NET_USER))) == NULL)
		return;

	AccountListTail(*pListHead);

	/* handle single user */
	if (szUser != NULL &&  _tcscmp(szUser, STEAMROLL_DEFAULT_USER) != 0)
	{
		InsertAccount(pListHead, szUser, 0, 0, 0, TIMEQ_FOREVER);
		return;
	}

	/* handle all users */
	/* todo: look into adding trust account filter */
	NetUserEnum(NULL, 2, FILTER_NORMAL_ACCOUNT, (LPBYTE*)&lpNtBuf, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, NULL);

	for (i = 0, lpAccounts = lpNtBuf; i < dwEntriesRead && i < dwTotalEntries && lpAccounts != NULL; ++i, ++lpAccounts)
		InsertAccount(pListHead, lpAccounts->usri2_name, lpAccounts->usri2_flags, lpAccounts->usri2_priv, lpAccounts->usri2_password_age, lpAccounts->usri2_acct_expires);

	if (lpNtBuf != NULL)
		NetApiBufferFree(lpNtBuf);
	
}