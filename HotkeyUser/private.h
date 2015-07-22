/*++

Copyright (c) 2009-2011  lazycat studio

Module Name:

    private.h

Abstract:


Author:

    lazy_cat

Environment:


Revision History:

    2011/10/22 13:29: create

--*/

DWORD 
WINAPI 
ThreadProc(
    LPVOID  lpParameter
    );

USHORT 
GetMakeCodeByVirtualKey(
    UINT nVirtualKey
    );

typedef struct _PARAM_INFO {
    HWND    hWnd;
    int     id;
    DWORD   (WINAPI *lpOnHotkeyCallback)(LPVOID, int);
    LPVOID  lpParameter;
} PARAM_INFO, *PPARAM_INFO;


#define release(memblock) { \
    if ((memblock)) \
    { \
        free((memblock)); \
        memblock = NULL; \
    } \
}
