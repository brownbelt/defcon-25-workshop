#pragma once

#include <Windows.h>

// include this file, and implement this function
void NTAPI tls_callback(PVOID DllHandle, DWORD dwReason, PVOID lpReserved);

#ifdef _WIN64
	#pragma comment (linker, "/INCLUDE:_tls_used")
	#pragma comment (linker, "/INCLUDE:tls_callback_func")

	#pragma const_seg(".CRT$XLF")
	EXTERN_C const PIMAGE_TLS_CALLBACK tls_callback_func = tls_callback;
	#pragma const_seg()
#else
	#pragma comment (linker, "/INCLUDE:__tls_used") 
	#pragma comment (linker, "/INCLUDE:_tls_callback_func")

	#pragma data_seg(".CRT$XLF")
	EXTERN_C PIMAGE_TLS_CALLBACK tls_callback_func = tls_callback;
	#pragma data_seg()
#endif
