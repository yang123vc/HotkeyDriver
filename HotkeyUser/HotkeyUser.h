/*++

Copyright (c) 2011  Shadow

Module Name:

    HotkeyUser.h

Abstract:

    Export function definition

Author:

    Shadow

Environment:

    User mode

Revision History:

--*/

#ifndef _HOTKEYSDK_H_
#define _HOTKEYSDK_H_

#ifdef __cplusplus
extern  "C"
{
#endif

typedef enum _HK_STATUS {
    HOTKEYSDK_SUCCESS       = 0,
    HOTKEYSDK_INVALIDVK     = 1,
    HOTKEYSDK_OVERFLOW      = 2,
    HOTKEYSDK_EXIST         = 3
} HK_STATUS;

HK_STATUS
WINAPI
ShadowRegisterHotKey(
    HWND    hWnd,           /* Not used */
    int     id,             /* Not used */
    UINT    fsModifiers,    /* Mask: support CTRL,ALT,SHIFT */
    UINT    nVirtualCode,
    DWORD   (WINAPI *lpOnHotkeyCallback)(LPVOID, int),
    LPVOID  lpParameter
    );

HK_STATUS 
WINAPI
ShadowUnRegisterAll();

#ifdef __cplusplus
    };
#endif

#endif  /* _HOTKEYSDK_H_ */
