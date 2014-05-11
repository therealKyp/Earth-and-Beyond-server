// LaunchNet7.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LaunchNet7.h"
#include "LaunchDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LaunchNet7

BEGIN_MESSAGE_MAP(LaunchNet7, CWinApp)
	//{{AFX_MSG_MAP(LaunchNet7)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LaunchNet7 construction

LaunchNet7::LaunchNet7()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only LaunchNet7 object

LaunchNet7 theApp;

/////////////////////////////////////////////////////////////////////////////
// LaunchNet7 initialization

BOOL LaunchNet7::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if (!strcmpi(theApp.m_lpCmdLine ,"patcher"))
	{
		LaunchDlg dlg;
		m_pMainWnd = &dlg;
		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
		}
		else if (nResponse == IDCANCEL)
		{
		}
	} else {
		MessageBox(NULL, "Please launch the patch utility first!", "Error", 0);
		// Execute patcher!
		char cmd[MAX_PATH];
		STARTUPINFO si = {NULL};
		PROCESS_INFORMATION pi = {NULL};

		sprintf(cmd, "EnBPatcher.exe");

		SetCurrentDirectory("../");
		CreateProcess("EnBPatcher.exe", cmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
