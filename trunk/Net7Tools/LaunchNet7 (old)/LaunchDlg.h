// LaunchDlg.h : header file
//

#if !defined(AFX_LAUNCHDLG_H__154D33BA_8009_4E50_8475_340F15D23948__INCLUDED_)
#define AFX_LAUNCHDLG_H__154D33BA_8009_4E50_8475_340F15D23948__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// LaunchDlg dialog

class LaunchDlg : public CDialog
{
// Construction
public:
	LaunchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(LaunchDlg)
	enum { IDD = IDD_LAUNCHER };
	CButton	m_UseUDPCtrl;
	CButton	m_UseTCPCtrl;
	CButton	m_UseMVAS;
	CButton	m_ClientDetours;
	CComboBox	m_ServerNameComboBox;
	CButton	m_AuthHttpsCtrl;
	CEdit	m_AuthPortCtrl;
	CButton	m_BrowseCtrl;
	CEdit	m_ClientPathCtrl;
	CButton	m_PlayButton;
	CButton	m_Net7SPCtrl;
	CButton	m_Net7MPCtrl;
	CButton	m_JavaMPCtrl;
	CButton	m_TestButton;
	CStatic	m_ServerStatusCtrl;
	CProgressCtrl	m_ProgressCtrl;
	CStatic	m_StatusCtrl;
	short	m_AuthPortValue;
	BOOL	m_AuthHttpsValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LaunchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(LaunchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEditchangeServerName();
	afx_msg void OnSelchangeServerName();
	afx_msg void OnTest();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnJavaMp();
	afx_msg void OnNet7Mp();
	afx_msg void OnNet7Sp();
	afx_msg void OnNet7cfg();
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnBrowse();
	afx_msg void OnHttps();
	afx_msg void OnKillfocusAuthPort();
	afx_msg void OnClientDetours();
	afx_msg void OnUseMvas();
	afx_msg void OnUseTcp();
	afx_msg void OnUseUdp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    void    LaunchNet7Server();
    void    LaunchClient();
	void	LaunchSocketTest();
    void    LaunchProxy();
    void    UpdateRegistry();
	bool	PatchAuthDLL();
    bool    UpdateFile(char *path, char *file);

    // "Validation" steps
    void    ContinueValidation();
    bool    ValidateProbeRegistry();

	void	SaveConfig();
	void	LoadConfig();

	void	SetAuthPort(int port);
	void	AppendToServerFile(char *file, char *hostname);
	void	ReadServerFile(char *file);
	void	GenerateENBPaths();
	void	CreateSQLCFG();
    void    CheckRG_RegData();
	bool	CheckFile(char *file);

private:
    long    m_ValidationTimer;
	bool	m_PassedValidation;
	unsigned long m_SQLProcessID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCHDLG_H__154D33BA_8009_4E50_8475_340F15D23948__INCLUDED_)
