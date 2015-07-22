#include "stdafx.h"
#include "DriverService.h"
#include <WinSvc.h>

BOOL
StartDriverService(IN LPCTSTR szServiceName, IN LPCTSTR szSysFilePath)
{
    SC_HANDLE hSCManager = NULL, hService = NULL;
    SERVICE_STATUS_PROCESS  ssStatus = { 0 };
    DWORD   dwBytesNeeded = 0;

    do 
    {
        hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (!hSCManager)
            break;
        hService = OpenService(hSCManager, szServiceName, SERVICE_ALL_ACCESS);
        if (!hService && ERROR_SERVICE_DOES_NOT_EXIST == GetLastError()) {
            hService = CreateService(hSCManager, szServiceName, szServiceName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, szSysFilePath, NULL, NULL, NULL, NULL, NULL);
            if (!hService)
                break;
        }

        if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE) &ssStatus,sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
            break;
        if (SERVICE_RUNNING != ssStatus.dwCurrentState) {
            StartService(hService, 0, NULL);
        }
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return TRUE;
    } while (FALSE);

    if (hService)
        CloseServiceHandle(hService);
    if (hSCManager)
        CloseServiceHandle(hSCManager);
    return FALSE;
}



VOID    DeleteDriverService(IN LPCTSTR szServiceName)
{
    SC_HANDLE hSCManager = NULL, hService = NULL;
    SERVICE_STATUS_PROCESS  ssStatus = { 0 };
    DWORD   dwBytesNeeded = 0;

    do 
    {
        if (!(hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
            break;
        if (!(hService = OpenService(hSCManager, szServiceName, SERVICE_ALL_ACCESS)))
            break;

        if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE) &ssStatus,sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
            break;
        if (SERVICE_STOPPED != ssStatus.dwCurrentState) {
            SERVICE_STATUS ServiceStatus = { 0 };
            ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus);
        }
        DeleteService(hService);
    } while (FALSE);

    if (hService)
        CloseServiceHandle(hService);
    if (hSCManager)
        CloseServiceHandle(hSCManager);
}