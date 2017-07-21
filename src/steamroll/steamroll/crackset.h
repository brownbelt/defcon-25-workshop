#pragma once

typedef struct _CRACK_SET
{
	DICTIONARY_INFO dictionary;
	PNET_USER lpUserList;
	DWORD dwUnsolved;
	DWORD dwPasswordAttempts;

	HANDLE hSetMutex;
} CRACK_SET, *PCRACK_SET;

VOID __fastcall InitCrackSet(_Inout_ PCRACK_SET lpCrackSet, _In_ PSTEAMROLL_OPTIONS options);
_Success_(return) BOOL __fastcall GetNextCrackSet(PCRACK_SET lpCrackSet, _In_opt_ LPVOID lpReserved, _Out_writes_z_(nPasswordSize) LPTSTR szPassword, _In_ int nPasswordSize);

VOID __fastcall FreeCrackSet(_In_ PCRACK_SET lpCrackset);