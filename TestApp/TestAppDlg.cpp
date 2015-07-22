
// TestAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestApp.h"
#include "TestAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "HotkeyUser.h"
#pragma comment(lib, "HotkeyUser.lib")

// CTestAppDlg dialog




CTestAppDlg::CTestAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestAppDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CTestAppDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CTestAppDlg message handlers

BOOL CTestAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI OnHotkeyCallback(LPVOID lpParameter, int nID)
{
    switch (nID)
    {
    case 1:
        ::MessageBox(NULL, _T("CTRL + A"), _T("Shadow"), MB_ICONINFORMATION);
        break;

    case 2:
        ::MessageBox(NULL, _T("CTRL + B"), _T("Shadow"), MB_ICONINFORMATION);
        break;

    case 3:
        ::MessageBox(NULL, _T("CTRL + C"), _T("Shadow"), MB_ICONINFORMATION);
        break;

    case 4:
        ::MessageBox(NULL, _T("CTRL + D"), _T("Shadow"), MB_ICONINFORMATION);
        break;
    }
    return 0;
}

void CTestAppDlg::OnBnClickedButtonTest()
{
    if (HOTKEYSDK_SUCCESS != ShadowRegisterHotKey(NULL, 1, MOD_CONTROL, 'A', OnHotkeyCallback, NULL))
    {
        MessageBox(_T("ShadowRegisterHotKey failed"));
    }
    if (HOTKEYSDK_SUCCESS != ShadowRegisterHotKey(NULL, 2, MOD_CONTROL | MOD_SHIFT, 'B', OnHotkeyCallback, NULL))
    {
        MessageBox(_T("ShadowRegisterHotKey failed"));
    }
    if (HOTKEYSDK_SUCCESS != ShadowRegisterHotKey(NULL, 3, MOD_CONTROL, 'C', OnHotkeyCallback, NULL))
    {
        MessageBox(_T("ShadowRegisterHotKey failed"));
    }
    if (HOTKEYSDK_SUCCESS != ShadowRegisterHotKey(NULL, 4, MOD_CONTROL | MOD_ALT, 'D', OnHotkeyCallback, NULL))
    {
        MessageBox(_T("ShadowRegisterHotKey failed"));
    }
}
