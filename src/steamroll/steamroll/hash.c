#include "stdafx.h"

_Success_(return) BOOL WINAPI NtlmHashW(_In_ const WCHAR *szKey, _Out_writes_(nBufferSize) WCHAR *szHexBuffer, _In_ _In_range_(NTLM_BUFFER_SIZE, INFINITE) size_t nBufferSize)
{
	BOOL success;
	size_t nLen;
	char *szAsciiKey;
	char szAsciiHexBuffer[NTLM_BUFFER_SIZE];

	nLen = wcslen(szKey) + 1;

	if ((szAsciiKey = (char *)malloc(nLen)) == NULL)
		return FALSE;

	wcstombs_s(&nLen, szAsciiKey, nLen, szKey, nLen + 1);
	success = NtlmHashA(szAsciiKey, szAsciiHexBuffer, nBufferSize);

	free(szAsciiKey);

	if (!success)
		return FALSE;

	mbstowcs_s(&nLen, szHexBuffer, NTLM_BUFFER_SIZE, szAsciiHexBuffer, NTLM_BUFFER_SIZE);

	return TRUE;
}

_Success_(return) BOOL WINAPI NtlmHashA(_In_ const char *szKey, _Out_writes_bytes_all_(nBufferSize) char *szHexBuffer, _In_ _In_range_(NTLM_BUFFER_SIZE, INFINITE) size_t nBufferSize)
{
	int i = 0;
	unsigned int nt_buffer[16];
	unsigned int output[4];
	static const char itoa16[17] = "0123456789abcdef";
	int length = strlen(szKey);

	if (nBufferSize < NTLM_BUFFER_SIZE)
		return FALSE;

	memset(nt_buffer, 0, 16 * 4);

	for (; i<length / 2; i++)
		nt_buffer[i] = szKey[2 * i] | (szKey[2 * i + 1] << 16);

	if (length % 2 == 1)
		nt_buffer[i] = szKey[length - 1] | 0x800000;
	else
		nt_buffer[i] = 0x80;

	nt_buffer[14] = length << 4;

	unsigned int a = NTLM_INIT_A;
	unsigned int b = NTLM_INIT_B;
	unsigned int c = NTLM_INIT_C;
	unsigned int d = NTLM_INIT_D;

	/* Round 1 */
	a += (d ^ (b & (c ^ d))) + nt_buffer[0]; a = (a << 3) | (a >> 29);
	d += (c ^ (a & (b ^ c))) + nt_buffer[1]; d = (d << 7) | (d >> 25);
	c += (b ^ (d & (a ^ b))) + nt_buffer[2]; c = (c << 11) | (c >> 21);
	b += (a ^ (c & (d ^ a))) + nt_buffer[3]; b = (b << 19) | (b >> 13);

	a += (d ^ (b & (c ^ d))) + nt_buffer[4]; a = (a << 3) | (a >> 29);
	d += (c ^ (a & (b ^ c))) + nt_buffer[5]; d = (d << 7) | (d >> 25);
	c += (b ^ (d & (a ^ b))) + nt_buffer[6]; c = (c << 11) | (c >> 21);
	b += (a ^ (c & (d ^ a))) + nt_buffer[7]; b = (b << 19) | (b >> 13);

	a += (d ^ (b & (c ^ d))) + nt_buffer[8]; a = (a << 3) | (a >> 29);
	d += (c ^ (a & (b ^ c))) + nt_buffer[9]; d = (d << 7) | (d >> 25);
	c += (b ^ (d & (a ^ b))) + nt_buffer[10]; c = (c << 11) | (c >> 21);
	b += (a ^ (c & (d ^ a))) + nt_buffer[11]; b = (b << 19) | (b >> 13);

	a += (d ^ (b & (c ^ d))) + nt_buffer[12]; a = (a << 3) | (a >> 29);
	d += (c ^ (a & (b ^ c))) + nt_buffer[13]; d = (d << 7) | (d >> 25);
	c += (b ^ (d & (a ^ b))) + nt_buffer[14]; c = (c << 11) | (c >> 21);
	b += (a ^ (c & (d ^ a))) + nt_buffer[15]; b = (b << 19) | (b >> 13);

	/* Round 2 */
	a += ((b & (c | d)) | (c & d)) + nt_buffer[0] + NTLM_SQRT_2; a = (a << 3) | (a >> 29);
	d += ((a & (b | c)) | (b & c)) + nt_buffer[4] + NTLM_SQRT_2; d = (d << 5) | (d >> 27);
	c += ((d & (a | b)) | (a & b)) + nt_buffer[8] + NTLM_SQRT_2; c = (c << 9) | (c >> 23);
	b += ((c & (d | a)) | (d & a)) + nt_buffer[12] + NTLM_SQRT_2; b = (b << 13) | (b >> 19);

	a += ((b & (c | d)) | (c & d)) + nt_buffer[1] + NTLM_SQRT_2; a = (a << 3) | (a >> 29);
	d += ((a & (b | c)) | (b & c)) + nt_buffer[5] + NTLM_SQRT_2; d = (d << 5) | (d >> 27);
	c += ((d & (a | b)) | (a & b)) + nt_buffer[9] + NTLM_SQRT_2; c = (c << 9) | (c >> 23);
	b += ((c & (d | a)) | (d & a)) + nt_buffer[13] + NTLM_SQRT_2; b = (b << 13) | (b >> 19);

	a += ((b & (c | d)) | (c & d)) + nt_buffer[2] + NTLM_SQRT_2; a = (a << 3) | (a >> 29);
	d += ((a & (b | c)) | (b & c)) + nt_buffer[6] + NTLM_SQRT_2; d = (d << 5) | (d >> 27);
	c += ((d & (a | b)) | (a & b)) + nt_buffer[10] + NTLM_SQRT_2; c = (c << 9) | (c >> 23);
	b += ((c & (d | a)) | (d & a)) + nt_buffer[14] + NTLM_SQRT_2; b = (b << 13) | (b >> 19);

	a += ((b & (c | d)) | (c & d)) + nt_buffer[3] + NTLM_SQRT_2; a = (a << 3) | (a >> 29);
	d += ((a & (b | c)) | (b & c)) + nt_buffer[7] + NTLM_SQRT_2; d = (d << 5) | (d >> 27);
	c += ((d & (a | b)) | (a & b)) + nt_buffer[11] + NTLM_SQRT_2; c = (c << 9) | (c >> 23);
	b += ((c & (d | a)) | (d & a)) + nt_buffer[15] + NTLM_SQRT_2; b = (b << 13) | (b >> 19);

	/* Round 3 */
	a += (d ^ c ^ b) + nt_buffer[0] + NTLM_SQRT_3; a = (a << 3) | (a >> 29);
	d += (c ^ b ^ a) + nt_buffer[8] + NTLM_SQRT_3; d = (d << 9) | (d >> 23);
	c += (b ^ a ^ d) + nt_buffer[4] + NTLM_SQRT_3; c = (c << 11) | (c >> 21);
	b += (a ^ d ^ c) + nt_buffer[12] + NTLM_SQRT_3; b = (b << 15) | (b >> 17);

	a += (d ^ c ^ b) + nt_buffer[2] + NTLM_SQRT_3; a = (a << 3) | (a >> 29);
	d += (c ^ b ^ a) + nt_buffer[10] + NTLM_SQRT_3; d = (d << 9) | (d >> 23);
	c += (b ^ a ^ d) + nt_buffer[6] + NTLM_SQRT_3; c = (c << 11) | (c >> 21);
	b += (a ^ d ^ c) + nt_buffer[14] + NTLM_SQRT_3; b = (b << 15) | (b >> 17);

	a += (d ^ c ^ b) + nt_buffer[1] + NTLM_SQRT_3; a = (a << 3) | (a >> 29);
	d += (c ^ b ^ a) + nt_buffer[9] + NTLM_SQRT_3; d = (d << 9) | (d >> 23);
	c += (b ^ a ^ d) + nt_buffer[5] + NTLM_SQRT_3; c = (c << 11) | (c >> 21);
	b += (a ^ d ^ c) + nt_buffer[13] + NTLM_SQRT_3; b = (b << 15) | (b >> 17);

	a += (d ^ c ^ b) + nt_buffer[3] + NTLM_SQRT_3; a = (a << 3) | (a >> 29);
	d += (c ^ b ^ a) + nt_buffer[11] + NTLM_SQRT_3; d = (d << 9) | (d >> 23);
	c += (b ^ a ^ d) + nt_buffer[7] + NTLM_SQRT_3; c = (c << 11) | (c >> 21);
	b += (a ^ d ^ c) + nt_buffer[15] + NTLM_SQRT_3; b = (b << 15) | (b >> 17);

	output[0] = a + NTLM_INIT_A;
	output[1] = b + NTLM_INIT_B;
	output[2] = c + NTLM_INIT_C;
	output[3] = d + NTLM_INIT_D;

	for (i = 0; i<4; i++)
	{
		int j = 0;
		unsigned int n = output[i];
		//iterate the bytes of the integer      
		for (; j<4; j++)
		{
			unsigned int convert = n % 256;
			szHexBuffer[i * 8 + j * 2 + 1] = itoa16[convert % 16];
			convert = convert / 16;
			szHexBuffer[i * 8 + j * 2 + 0] = itoa16[convert % 16];
			n = n / 256;
		}
	}

	szHexBuffer[32] = 0;

	return TRUE;
}