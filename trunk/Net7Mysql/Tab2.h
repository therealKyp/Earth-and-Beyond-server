#if !defined(AFX_TAB2_H__CC827765_B7C2_429E_8AA7_E892F411234D__INCLUDED_)
#define AFX_TAB2_H__CC827765_B7C2_429E_8AA7_E892F411234D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tab2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Tab2 dialog

class Tab2 : public CDialog
{
// Construction
public:
	Tab2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Tab2)
	enum { IDD = IDD_TAB1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CEdit		m_Pass;
	CComboBox	m_User;
	CComboBox	m_Type;
	CEdit		m_LastLogin;

	struct Accounts {
		int		AccountID;
		CString	LastLogin;
		CString Username;
		CString Password;
		int		Type;
	};

	Accounts AccList[30000];
	int		 m_MaxAccounts;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Tab2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void	LoadUsers();			// Loads users from MySQL
	int		GetIndex(int Type);
	int		GetType(int Index);
	void	AppendToServerFile(char *file, char *hostname);

	// Generated message map functions
	//{{AFX_MSG(Tab2)
	afx_msg void OnAddUser();
	afx_msg void OnSelchangeUser();
	afx_msg void OnUpdateUsers();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeAccountType();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB2_H__CC827765_B7C2_429E_8AA7_E892F411234D__INCLUDED_)
