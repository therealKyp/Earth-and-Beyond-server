// Net7MysqlDlg.h : header file
//

#if !defined(AFX_NET7MYSQLDLG_H__1298B390_1CC4_4685_8164_A6655A27359C__INCLUDED_)
#define AFX_NET7MYSQLDLG_H__1298B390_1CC4_4685_8164_A6655A27359C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./mysql/mysqlplus.h"
#include "Tab2.h"
#include "ItemBase.h"

#define STARTUP_SERVER		"localhost:3307"
#define	SERVER_FILE			"mysql_servers.txt"

extern sql_connection_c m_MySQL_Connect;
extern bool	m_Connected;		// True if connected to mysql!

/////////////////////////////////////////////////////////////////////////////
// CNet7MysqlDlg dialog

class CNet7MysqlDlg : public CDialog
{
// Construction
public:
	CNet7MysqlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNet7MysqlDlg)
	enum { IDD = IDD_NET7MYSQL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CComboBox	m_Mysql_Server;
	CButton		m_ConnectButtion;
	CEdit		m_Mysql_User;
	CEdit		m_Mysql_Password;
	CEdit		m_Mysql_Database;
	CEdit		m_Debug;
	CEdit		m_MySQL_File;
	CTabCtrl	m_Tabs;

	// Tabs
	CDialog	  * m_tabPages[5];
	CString		m_tabText[5];
	int			m_tabCurrent;
	int			m_nNumberOfPages;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNet7MysqlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	void	UpdateTabs();
	void	AppendToServerFile(char *file, char *hostname);
	void	ReadServerFile(char *file);

	// Generated message map functions
	//{{AFX_MSG(CNet7MysqlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnConnect();
	afx_msg void OnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NET7MYSQLDLG_H__1298B390_1CC4_4685_8164_A6655A27359C__INCLUDED_)
