/*++

Copyright (c) 2011  3NODSUZ

Module Name:

    HotkeyUser.h

Abstract:


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
    HOTKEYSDK_OVERFLOW      = 2
} HK_STATUS;

HK_STATUS
WINAPI
ShadowRegisterHotKey(
    HWND    hWnd,
    int     id,
    UINT    fsModifiers,
    UINT    nMakeCode,
    DWORD   (WINAPI *lpOnHotkeyCallback)(LPVOID, int),
    LPVOID  lpParameter
    );

#ifdef __cplusplus
    };
#endif

    
#endif  /* _HOTKEYSDK_H_ */