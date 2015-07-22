How to use this sdk:

1. include HotkeyUser.h into your project

2. link with HotkeyUser.lib

3. E.G.:
DWORD WINAPI OnHotkeyCallback(LPVOID lpParameter, int nID)
{
    switch (nID)
    {
    case 1:
        ::MessageBox(NULL, _T("CTRL + A"), _T("Shadow"), MB_ICONINFORMATION);
        break;

    case 2:
        ::MessageBox(NULL, _T("CTRL + SHIFT + B"), _T("Shadow"), MB_ICONINFORMATION);
        break;

    case 3:
        ::MessageBox(NULL, _T("CTRL + C"), _T("Shadow"), MB_ICONINFORMATION);
        break;

    case 4:
        ::MessageBox(NULL, _T("CTRL + ALT + D"), _T("Shadow"), MB_ICONINFORMATION);
        break;
    }
    return 0;
}

void CTestAppDlg::OnBnClickedButtonTest()
{
     ShadowRegisterHotKey(NULL, 1, MOD_CONTROL, 'A', OnHotkeyCallback, NULL);
     ShadowRegisterHotKey(NULL, 2, MOD_CONTROL | MOD_SHIFT, 'B', OnHotkeyCallback, NULL);
     ShadowRegisterHotKey(NULL, 3, MOD_CONTROL, 'C', OnHotkeyCallback, NULL);
     ShadowRegisterHotKey(NULL, 4, MOD_CONTROL | MOD_ALT, 'D', OnHotkeyCallback, NULL);
}

4. Release your application with HotkeyUser.dll & HotKeyKrnl.sys