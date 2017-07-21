#pragma once

#define NTLM_INIT_A 0x67452301
#define NTLM_INIT_B 0xefcdab89
#define NTLM_INIT_C 0x98badcfe
#define NTLM_INIT_D 0x10325476

#define NTLM_SQRT_2 0x5a827999
#define NTLM_SQRT_3 0x6ed9eba1

#define NTLM_BUFFER_SIZE 33

#ifdef UNICODE
#define NtlmHash NtlmHashW
#else
#define NtlmHash NtlmHashA
#endif

_Success_(return) BOOL WINAPI NtlmHashW(_In_ const WCHAR *szKey, _Out_writes_(nBufferSize) WCHAR *szHexBuffer, _In_ _In_range_(NTLM_BUFFER_SIZE, INFINITE) size_t nBufferSize);
_Success_(return) BOOL WINAPI NtlmHashA(_In_ const char *szKey, _Out_writes_bytes_all_(nBufferSize) char *szHexBuffer, _In_ _In_range_(NTLM_BUFFER_SIZE, INFINITE) size_t nBufferSize);