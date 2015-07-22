#include "stdafx.h"
#include "private.h"
#include "../Common/ioctlproto.h"

HANDLE              g_hThread           = NULL;

DWORD               g_dwParamInfoCount  = 0;
PPARAM_INFO         g_pParamInfoList    = NULL;
PIOCTL_INPUTDATA    g_pIoctlInputData   = NULL;
HANDLE              g_hDevice           = INVALID_HANDLE_VALUE;
HANDLE              g_hShareEvent       = NULL;
BOOL                g_bIsX64            = FALSE;