#pragma once

typedef struct _STEAMROLL_THREAD_DATA
{
	STEAMROLL_OPTIONS options;
	CRACK_SET crackset;
} STEAMROLL_THREAD_DATA, *PSTEAMROLL_THREAD_DATA;

BOOL __fastcall ScheduleThreads(_In_ PSTEAMROLL_THREAD_DATA lpThreadData);