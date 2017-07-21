#pragma once

#include <windows.h>
#include <Wininet.h>

#pragma comment(lib, "wininet.lib")

static BOOL http_request(LPCSTR host, WORD port, BOOL secure, LPCSTR verb, LPCSTR path, LPCSTR szHeaders, SIZE_T nHeaderSize,
	LPCSTR postData, SIZE_T nPostDataSize, char **data, LPDWORD dwDataSize)
{
	HINTERNET hIntSession = NULL;
	HINTERNET hHttpSession = NULL;
	HINTERNET hHttpRequest = NULL;
	DWORD dwFlags = 0;

	BOOL ret = FALSE;

	do
	{
		hIntSession = InternetOpenA("Mozilla 5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

		if (NULL == hIntSession)
			break;

		hHttpSession = InternetConnectA(hIntSession, host, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, (DWORD_PTR)NULL);

		if (NULL == hHttpSession)
			break;

		dwFlags |= INTERNET_FLAG_RELOAD;
		if (secure)
			dwFlags |= INTERNET_FLAG_SECURE;

		hHttpRequest = HttpOpenRequestA(hHttpSession, verb, path, 0, 0, 0, dwFlags, 0);

		if (NULL == hHttpRequest)
			break;

		if (!HttpSendRequestA(hHttpRequest, szHeaders, (DWORD)nHeaderSize, (LPVOID)postData, (DWORD)nPostDataSize))
			break;

		CHAR szBuffer[1024];
		CHAR *output = (CHAR*)malloc(1024);
		DWORD dwRead = 0;
		DWORD dwTotalBytes = 0;

		memset(output, 0, 1024);
		memset(szBuffer, 0, sizeof(szBuffer));

		while (InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead)
		{
			DWORD dwOffset = dwTotalBytes;
			dwTotalBytes += dwRead;

			output = (CHAR*)realloc(output, dwTotalBytes);
			memcpy(output + dwOffset, szBuffer, dwRead);

			memset(szBuffer, 0, sizeof(szBuffer));
			dwRead = 0;
		}

		*data = output;
		*dwDataSize = dwTotalBytes;

		ret = TRUE;
	} while (0);

	if (hHttpRequest)
		InternetCloseHandle(hHttpRequest);

	if (hHttpSession)
		InternetCloseHandle(hHttpSession);

	if (hIntSession)
		InternetCloseHandle(hIntSession);

	return ret;
}