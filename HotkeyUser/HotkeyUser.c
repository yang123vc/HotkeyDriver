/*++

Copyright (c) 2011  3NODSUZ

Module Name:

    HotkeyUser.c

Abstract:


Author:

    Shadow

Environment:


Revision History:

--*/

#include "stdafx.h"
#include "HotkeyUser.h"
#include "private.h"
#include "../Common/ioctlproto.h"
#include "global.h"



HK_STATUS 
WINAPI
ShadowRegisterHotKey(
    HWND    hWnd,
    int     id,
    UINT    fsModifiers,
    UINT    nVirtualCode,
    DWORD   (WINAPI *lpOnHotkeyCallback)(LPVOID, int),
    LPVOID  lpParameter
    )
{
    DWORD   dwBytesReturned = 0;
    DWORD   i = 0;

    if (g_bIsX64)
    {
        return HOTKEYSDK_SUCCESS;
    }

    if (!(nVirtualCode = GetMakeCodeByVirtualKey(nVirtualCode)))
    {
        return HOTKEYSDK_INVALIDVK;
    }

    // Whether has been registered
    for (i = 0; i < g_dwParamInfoCount; i++)
    {
        if (g_pIoctlInputData[i].fsModifiers == fsModifiers && g_pIoctlInputData[i].nMakeCode == nVirtualCode)
        {
            return HOTKEYSDK_EXIST;
        }
    }
    
    // Construct the hot key list
    g_dwParamInfoCount++;
    g_pParamInfoList = g_pParamInfoList ? (realloc(g_pParamInfoList, sizeof(PARAM_INFO) * g_dwParamInfoCount)) : (malloc(sizeof(PARAM_INFO) * g_dwParamInfoCount));
    if (!g_pParamInfoList)
    {
        g_dwParamInfoCount--;
        return HOTKEYSDK_OVERFLOW;
    }
    g_pIoctlInputData = g_pIoctlInputData ? (realloc(g_pIoctlInputData, sizeof(IOCTL_INPUTDATA) * g_dwParamInfoCount)) : (malloc(sizeof(IOCTL_INPUTDATA) * g_dwParamInfoCount));
    if (!g_pIoctlInputData)
    {
        g_dwParamInfoCount--;
        return HOTKEYSDK_OVERFLOW;
    }

    g_pIoctlInputData[g_dwParamInfoCount - 1].fsModifiers = fsModifiers;
    g_pIoctlInputData[g_dwParamInfoCount - 1].nMakeCode = nVirtualCode;
    g_pParamInfoList[g_dwParamInfoCount - 1].hWnd = hWnd;
    g_pParamInfoList[g_dwParamInfoCount - 1].id = id;
    g_pParamInfoList[g_dwParamInfoCount - 1].lpOnHotkeyCallback = lpOnHotkeyCallback;
    g_pParamInfoList[g_dwParamInfoCount - 1].lpParameter = lpParameter;

    // Send the list data to driver
    DeviceIoControl(g_hDevice, IOCTL_SETHOTKEYLIST, g_pIoctlInputData, sizeof(IOCTL_INPUTDATA) * g_dwParamInfoCount, NULL, 0, &dwBytesReturned, NULL);
    return 0;
}



HK_STATUS 
WINAPI
ShadowUnRegisterAll()
{
    DWORD   dwBytesReturned = 0;
    g_dwParamInfoCount = 0;
    if (g_bIsX64)
    {
        return HOTKEYSDK_SUCCESS;
    }

    release(g_pIoctlInputData);
    release(g_pParamInfoList);
    DeviceIoControl(g_hDevice, IOCTL_CLEARLIST, NULL, 0, NULL, 0, &dwBytesReturned, NULL);
    return HOTKEYSDK_SUCCESS;
}



DWORD
WINAPI 
ThreadProc(
    LPVOID  lpParameter
    )
{
    DWORD   dwHotkeyIndex, dwBytesReturned;
    while (TRUE)
    {
        dwHotkeyIndex   = -1;
        dwBytesReturned = 0;
        if (WAIT_OBJECT_0 == WaitForSingleObject(g_hShareEvent, INFINITE))
        {
            if (DeviceIoControl(g_hDevice, IOCTL_GETHOTKEYINDEX, NULL, 0, &dwHotkeyIndex, sizeof(DWORD), &dwBytesReturned, NULL))
            {
                if (dwBytesReturned)
                {
                    if (dwHotkeyIndex < g_dwParamInfoCount)
                    {
                        g_pParamInfoList[dwHotkeyIndex].lpOnHotkeyCallback(g_pParamInfoList[dwHotkeyIndex].lpParameter, g_pParamInfoList[dwHotkeyIndex].id);
                    }
                }
            }
        }
    }
    return 0;
}


USHORT GetMakeCodeByVirtualKey(UINT nVirtualKey)
{
    int     i = 0;
    struct { 
        UINT    vk;
        USHORT  ScanCode;
    } KeyTab[] =
    {
        'A',            0x1E,
        'B',            0x30,
        'C',            0x2E,
        'D',            0x20,
        'E',            0x12,
        'F',            0x21,
        'G',            0x22,
        'H',            0x23,
        'I',            0x17,
        'J',            0x24,
        'K',            0x25,
        'L',            0x26,
        'M',            0x32,
        'N',            0x31,
        'O',            0x18,
        'P',            0x19,
        'Q',            0x10,
        'R',            0x13,
        'S',            0x1F,
        'T',            0x14,
        'U',            0x16,
        'V',            0x2F,
        'W',            0x11,
        'X',            0x2D,
        'Y',            0x15,
        'Z',            0x2C,
        '0',            0x0B,   
        '1',            0x02,
        '2',            0x03,
        '3',            0x04,
        '4',            0x05,
        '5',            0x06,
        '6',            0x07,
        '7',            0x08,
        '8',            0x09,
        '9',            0x0A,
        VK_NUMPAD0,     0x52,
        VK_NUMPAD1,     0x4F,
        VK_NUMPAD2,     0x50,
        VK_NUMPAD3,     0x51,
        VK_NUMPAD4,     0x4B,
        VK_NUMPAD5,     0x4C,
        VK_NUMPAD6,     0x4D,
        VK_NUMPAD7,     0x47,
        VK_NUMPAD8,     0x48,
        VK_NUMPAD9,     0x49,
        VK_MULTIPLY,    0x37,
        VK_ADD,         0x4E,
        VK_SUBTRACT,    0x4A,
        VK_DECIMAL,     0x53,
        VK_DIVIDE,      0x35,
        VK_F1,          0x3B,
        VK_F2,          0x3C,
        VK_F3,          0x3D,
        VK_F4,          0x3E,
        VK_F5,          0x3F,
        VK_F6,          0x40,
        VK_F7,          0x41,
        VK_F8,          0x42,
        VK_F9,          0x43,
        VK_F10,         0x44,
        VK_F11,         0x57,
        VK_SPACE,       0x39,
    };
    for (i = 0; i < sizeof(KeyTab) / sizeof(KeyTab[0]); i++)
        if (nVirtualKey == KeyTab[i].vk)
            return KeyTab[i].ScanCode;
    return 0;
}