// LaunchNet7.h : main header file for the LAUNCHNET7 application
//

#if !defined(AFX_LAUNCHNET7_H__C4B35DDC_6E8B_4B3E_A13C_1FC800524FF6__INCLUDED_)
#define AFX_LAUNCHNET7_H__C4B35DDC_6E8B_4B3E_A13C_1FC800524FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// LaunchNet7:
// See LaunchNet7.cpp for the implementation of this class
//

class LaunchNet7 : public CWinApp
{
public:
	LaunchNet7();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LaunchNet7)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(LaunchNet7)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCHNET7_H__C4B35DDC_6E8B_4B3E_A13C_1FC800524FF6__INCLUDED_)
