#pragma once

#include <winternl.h>

typedef void *PEPROCESS;

typedef PVOID(NTAPI * MmGetSystemRoutineAddress_t)(PUNICODE_STRING);
