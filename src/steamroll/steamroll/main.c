#include "stdafx.h"

_Success_(return == EXIT_SUCCESS) 
int _cdecl _tmain(_In_ int argc, _In_ _TCHAR *argv[])
{
	int ret = EXIT_FAILURE;
	STEAMROLL_THREAD_DATA srThreadData;
	
	if (!ParseCommandLine(argc, argv, &srThreadData.options))
	{
		ShowCommandHelp();
		return srThreadData.options.bAVCheck ? EXIT_FAILURE : EXIT_SUCCESS;
	}
		
	if (!srThreadData.options.bLockouts && !CheckLockouts())
		return EXIT_FAILURE;

	InitCrackSet(&srThreadData.crackset, &srThreadData.options);
	ret = ScheduleThreads(&srThreadData) ? EXIT_SUCCESS : EXIT_FAILURE;
	FreeCrackSet(&srThreadData.crackset);
	
	return ret;
}