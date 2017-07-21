#pragma once

#include <winsdkver.h>

/* Target XP SP0. Win2k and earlier are unsupported by Microsoft */
#define NTDDI_VERSION	NTDDI_WINXP
#define WINVER			_WIN32_WINNT_WINXP
#define _WIN32_WINNT	_WIN32_WINNT_WINXP

#include <SDKDDKVer.h>
