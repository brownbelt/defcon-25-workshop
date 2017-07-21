#include "stdafx.h"

BOOL __fastcall ReadFileAllAscii(_In_ const WCHAR *szFile, _Outptr_ char **szBuffer, _Out_ size_t *nBufferSize)
{
	FILE *pFile;
	size_t nConverted;
	char szAsciiFile[MAX_PATH];

	if (wcstombs_s(&nConverted, szAsciiFile, MAX_PATH, szFile, MAX_PATH) != 0)
		return FALSE;

	if (fopen_s(&pFile, szAsciiFile, "r") != 0)
		return FALSE;

	fseek(pFile, 0, SEEK_END);
	*nBufferSize = ftell(pFile) + 1;

	if ((*szBuffer = (char *)malloc(*nBufferSize)) == NULL)
		return FALSE;

	fseek(pFile, 0, SEEK_SET);
	fread(*szBuffer, 1, (*nBufferSize) - 1, pFile);
	fclose(pFile);

	(*szBuffer)[*nBufferSize - 1] = '\0';

	return TRUE;
}

VOID __fastcall FreeFileAllAscii(_In_ char *szBuffer)
{
	free((PVOID)szBuffer);
}

BOOL __fastcall WriteLineFileW(_In_ const WCHAR *szFile, _In_ const WCHAR *szInfo)
{
	FILE *pFile;

	if (_wfopen_s(&pFile, szFile, L"a") != 0)
		return FALSE;

	if (pFile == NULL)
		return FALSE;

	fwprintf_s(pFile, L"%s\n", szInfo);

	fclose(pFile);

	return TRUE;
}

