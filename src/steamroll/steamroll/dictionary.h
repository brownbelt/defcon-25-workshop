#pragma once

typedef struct _DICTIONARY_INFO
{
	LPVOID lpStartAddress;
	LPVOID lpEndAddress;
	LPVOID lpCurrentAddress;
	DWORD dwBytes;
} DICTIONARY_INFO, *PDICTIONARY_INFO;

_Success_(return) BOOL __fastcall InitDictionary(_Inout_ PDICTIONARY_INFO lpDictionary, _In_opt_ LPCTSTR szFile);
_Success_(return) BOOL __fastcall NextDictionaryWord(_Inout_ PDICTIONARY_INFO lpDictionary, _Out_writes_(nWordSize) LPTSTR szWord, _In_ int nWordSize);