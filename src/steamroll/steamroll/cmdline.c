#include "stdafx.h"

_Success_(return) static BOOL __forceinline ArgumentBoundsCheck(_In_ int argc, _Inout_ int *index)
{
	++(*index);
	if (argc == *index)
		return FALSE;

	return TRUE;
}

/* suppress as _tcscpy_s will add a terminator */
#pragma warning(suppress : 6054)
_Success_(return) static BOOL __forceinline ParseStringArgument(_In_ int argc, _Inout_ int *index, _In_reads_z_(nValSize) _TCHAR *argv[], _Out_writes_(nValSize) LPTSTR szValue, _In_ unsigned int nValSize)
{
	if (!ArgumentBoundsCheck(argc, index))
		return FALSE;

	_tcscpy_s(szValue, nValSize, argv[*index]);
	
	return TRUE;
}

_Success_(return) static BOOL __forceinline ParseIntArgument(_In_ int argc, _Inout_ int *index, _In_ _TCHAR *argv[], _Out_ int *nValue)
{
	if (!ArgumentBoundsCheck(argc, index))
		return FALSE;

	*nValue = _ttoi(argv[*index]);

	return TRUE;
}

_Success_(return) _Check_return_ BOOL __fastcall ParseCommandLine(_In_ int argc, _In_ _TCHAR *argv[], _Out_ PSTEAMROLL_OPTIONS options)
{
	int i;

	options->nThreads = STEAMROLL_DEFAULT_THREADS;
	options->bVerbose = STEAMROLL_DEFAULT_VERBOSE;
	options->bDisabledUsers = STEAMROLL_DEFAULT_DISABLEDUSERS;
	options->bExpiredPasswords = STEAMROLL_DEFAULT_EXPIREDPASSWORDS;
	options->bLockouts = STEAMROLL_DEFAULT_LOCKOUTS;
	options->bAVCheck = STEAMROLL_DEFAULT_AVCHECK;

	_tcscpy_s(options->szFile, _countof(options->szFile), STEAMROLL_DEFAULT_FILE);
	_tcscpy_s(options->szUser, _countof(options->szUser), STEAMROLL_DEFAULT_USER);
	_tcscpy_s(options->szOutput, _countof(options->szOutput), STEAMROLL_DEFAULT_USER);

	for (i = 1; i < argc; ++i)
	{
		switch ((BYTE)*++argv[i])
		{
		case 'h':
			return FALSE;
		case 't':
			if (!ParseIntArgument(argc, &i, argv, (int *)&options->nThreads))
				return FALSE;
			break;
		case 'u':
			if (!ParseStringArgument(argc, &i, argv, options->szUser, _countof(options->szUser)))
				return FALSE;
			break;
		case 'f':
			if (!ParseStringArgument(argc, &i, argv, options->szFile, _countof(options->szFile)))
				return FALSE;
			break;
		case 'o':
			if (!ParseStringArgument(argc, &i, argv, options->szOutput, _countof(options->szOutput)))
				return FALSE;
			break;
		case 'q':
			options->bVerbose = FALSE;
			break;
		case 'l':
			options->bLockouts = TRUE;
			break;
		/*
		case 'd':
			options->bDisabledUsers = TRUE;
			break;
		case 'e':
			options->bExpiredPasswords = TRUE;
			break;
		*/
		case 'R':
			options->bAVCheck = TRUE;
			break;
		}
	}

	return options->bAVCheck;
}

VOID WINAPI ShowCommandHelp(void)
{
	printf("\nUsage: steamroll.exe -R [flags]\n\n");
	printf("\tFlag\tArg\tDescription\t\t\tDefault\n");
	printf("\t------------------------------------------------------------\n");
	printf("\t-R\t\tanti-AV, MUST pass to run\tOFF\n");
	printf("\t-t\tbyte\tnumber of worker threads\t%d\n", STEAMROLL_DEFAULT_THREADS);
	printf("\t-f\tstr\tcustom pw dictionary file\trockyou.txt\n");
	printf("\t-o\tstr\twrite cracked users to file\tNULL\n");
	printf("\t-u\tstr\tspecify a user to target\tAll users\n");
	printf("\t-q\t\tdisable verbose progress output\tOFF\n");
	printf("\t-l\t\tbypass lockout policy check\tOFF\n");
	/*
	printf("\t-d\t\tcrack disabled users\tOFF\n");
	printf("\t-e\t\tcrack expired passwords\tOFF\n");
	*/
}