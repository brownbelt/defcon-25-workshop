#include "stdafx.h"

static BOOL __fastcall InitDictionaryFromFile(_Inout_ PDICTIONARY_INFO lpDictionary, _In_ LPCTSTR szFile)
{
	if (!ReadFileAllAscii(szFile, (char**)&lpDictionary->lpStartAddress, &lpDictionary->dwBytes))
		return FALSE;

	lpDictionary->lpCurrentAddress = lpDictionary->lpStartAddress;
	lpDictionary->lpEndAddress = (char *)lpDictionary->lpStartAddress + lpDictionary->dwBytes;

	return TRUE;
}

_Success_(return) BOOL __fastcall InitDictionary(_Inout_ PDICTIONARY_INFO lpDictionary, _In_opt_ LPCTSTR szFile)
{
	HMODULE hModule;
	HRSRC hResource;
	HGLOBAL hResourceData;

	if (szFile != NULL && _tcscmp(szFile, STEAMROLL_DEFAULT_FILE) != 0)
		return InitDictionaryFromFile(lpDictionary, szFile);

	hModule = GetModuleHandle(NULL);

	if ((hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_TEXTFILE1), MAKEINTRESOURCE(TEXTFILE))) == NULL)
		return FALSE;

	if ((hResourceData = LoadResource(hModule, hResource)) == NULL)
		return FALSE;

	lpDictionary->lpStartAddress = LockResource(hResourceData);
	lpDictionary->lpCurrentAddress = lpDictionary->lpStartAddress;

	lpDictionary->dwBytes = SizeofResource(hModule, hResource);
	lpDictionary->lpEndAddress = (char *)lpDictionary->lpStartAddress + lpDictionary->dwBytes;

	return TRUE;
}

/* suppress as code will add a terminator */
#pragma warning(suppress : 6054)
_Success_(return) BOOL __fastcall NextDictionaryWord(_Inout_ PDICTIONARY_INFO lpDictionary, _Out_writes_(nWordSize) LPTSTR szWord, _In_ int nWordSize)
{
	char *pch;
	const char *pDictionaryString;
	size_t nReset = 1;
	int cbOffset;

	if (lpDictionary->lpCurrentAddress > lpDictionary->lpEndAddress)
		return FALSE;

	pDictionaryString = (const char *)lpDictionary->lpCurrentAddress;

	if ((pch = strchr(pDictionaryString, '\n')) == NULL)
		if ((pch = strchr(pDictionaryString, '\0')) == NULL)
			return FALSE;

	if (*(pch - 1) == '\r')
	{
		--(pch);
		++nReset;
	}

	cbOffset = (int)(pch - pDictionaryString);

	MultiByteToWideChar(CP_UTF8, 0, pDictionaryString, cbOffset, szWord, nWordSize);

	/* todo: resource 0-bytes for faster implementation */
#ifdef UNICODE
	*(uint16_t *)(szWord + cbOffset) = (uint16_t)0x00000;
#else
	szWord[cbOffset] = '\0';
#endif

	lpDictionary->lpCurrentAddress = (LPVOID)(pch + nReset);

	return TRUE;
}