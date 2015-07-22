#include "stdafx.h"
#include "private.h"
#include "../Common/ioctlproto.h"
#include "global.h"
#include "DriverService.h"
#pragma  comment(lib, "Advapi32.lib")


BOOL 
WINAPI
DllMain(
    HINSTANCE   hinstDLL,
    DWORD       fdwReason,
    LPVOID      lpvReserved
    )
{
    DWORD   dwBytesReturned = 0;
    WCHAR   szSysPath[MAX_PATH] = { 0 };
    int     nLength = 0;
    int     i       = 0;
    SYSTEM_INFO SystemInfo;

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        GetNativeSystemInfo(&SystemInfo);
        g_bIsX64 = PROCESSOR_ARCHITECTURE_AMD64 == SystemInfo.wProcessorArchitecture;
        if (g_bIsX64)
        {
            return TRUE;
        }


        // Start driver
        GetModuleFileName(NULL, szSysPath, MAX_PATH);
        nLength = wcslen(szSysPath);
        for (i = nLength - 1; i >= 0; i--)
        {
            if ('\\' == szSysPath[i])
            {
                szSysPath[i] = 0;
                break;
            }
        }
        wcscat_s(szSysPath, MAX_PATH, _T("\\HotKeyKrnl.sys"));
        if (!StartDriverService(_T("HotkeyKrnlSvc"), szSysPath))
        {
            return FALSE;
        }
        
        // Open device
        g_hDevice = CreateFile(FILE_NAME, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (INVALID_HANDLE_VALUE == g_hDevice)
        {
            return FALSE;
        }
        
        // Create an event,then send its handle to driver
        g_hShareEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        DeviceIoControl(g_hDevice, IOCTL_INITEVENT, &g_hShareEvent, sizeof(HANDLE), NULL, 0, &dwBytesReturned, NULL);
        
        // Create a thread do ioctl
        if (!(g_hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL)))
        {
            return FALSE;
        }
        break;

    case DLL_PROCESS_DETACH:
        if (g_bIsX64)
        {
            return TRUE;
        }

        if (INVALID_HANDLE_VALUE != g_hDevice)
        {
            CloseHandle(g_hDevice); // -> IRP_MJ_CLOSE
        }
        release(g_pParamInfoList);
        release(g_pIoctlInputData);

        // Stop the driver?
        break;
    }
    return TRUE;
}
