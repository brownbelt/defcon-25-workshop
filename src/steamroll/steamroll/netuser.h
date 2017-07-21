#pragma once

/* Max username on NT szUser is 20 TCHAR */
#define NET_USER_BUFFERSIZE 21 * sizeof(TBYTE)

/* Linked list iterator */
#define NET_USER_LOOP(pEntry, pListHead) for (pEntry = pListHead; pEntry->lpNext != NULL; pEntry = pEntry->lpNext)

typedef struct _NET_USER
{
	struct _NET_USER *lpNext;
	TCHAR szUser[NET_USER_BUFFERSIZE];
	HANDLE hToken;
	BOOL bActive;						/* likely more performant than using a semaphore (atomic vs. syscall context switch) */
	DWORD dwFlags;
	DWORD dwAccountType;
	DWORD dwPasswordAge;
	DWORD dwAccountExpires;
} NET_USER, *PNET_USER;

VOID __fastcall InitAccountList(_Inout_ PNET_USER *pListHead, _In_opt_ LPCTSTR szUser);
VOID __fastcall FreeAccountList(_In_ PNET_USER pListHead);