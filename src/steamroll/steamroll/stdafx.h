#pragma once

#define STRICT 
#define WIN32_LEAN_AND_MEAN

#include "targetver.h"
#include "resource.h"

#include <Windows.h>
#include <LM.h>
/* #include <Ntsecapi.h> /* */
/* #include <subauth.h> /* */

#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "advapi32.lib") 
/* #pragma comment(lib, "crypt32.lib") /* */

#include <tchar.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

/* #define ZLIB_CONST /* */

/* #include "zlib/zlib.h" /* */
/* #pragma comment(lib, "zlib.lib") /* */

#include "options.h"
#include "cmdline.h"
/* #include "inflate.h" */
#include "policy.h"
#include "hash.h"
#include "fileio.h"
#include "netuser.h"
#include "dictionary.h"
#include "crackset.h"
#include "threadsched.h"
