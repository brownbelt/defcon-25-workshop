#pragma once

_Success_(return) _Check_return_  BOOL __fastcall ParseCommandLine(_In_ int argc, _In_ _TCHAR *argv[], _Out_ PSTEAMROLL_OPTIONS options);

VOID WINAPI ShowCommandHelp(void);
VOID WINAPI ShowConsoleMessage(_In_ LPCTSTR szMsg, _In_opt_ DWORD dwColor);