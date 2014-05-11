// Net7Mysql.h : main header file for the NET7MYSQL application
//

#if !defined(AFX_NET7MYSQL_H__BC15AD65_B050_47D3_8105_CB4AA0FBEF53__INCLUDED_)
#define AFX_NET7MYSQL_H__BC15AD65_B050_47D3_8105_CB4AA0FBEF53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "./mysql/mysqlplus.h"
#include "resource.h"		// main symbols

#define MYSQL_BASE_BIN		"c:\\net7\\mysql"

/////////////////////////////////////////////////////////////////////////////
// CNet7MysqlApp:
// See Net7Mysql.cpp for the implementation of this class
//

class CNet7MysqlApp : public CWinApp
{
public:
	CNet7MysqlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNet7MysqlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNet7MysqlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NET7MYSQL_H__BC15AD65_B050_47D3_8105_CB4AA0FBEF53__INCLUDED_)
