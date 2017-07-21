#pragma once


BOOL __fastcall ReadFileAllAscii(_In_ const WCHAR *szFile, _Outptr_ char **szBuffer, _Out_ size_t *nBufferSize);
VOID __fastcall FreeFileAllAscii(_In_ char *szBuffer);

BOOL __fastcall WriteLineFileW(_In_ const WCHAR *szFile, _In_ const WCHAR *szInfo);
