#if !defined(AFX_OPTIONSDLG_H__3BBDC969_E1FB_4E59_96A9_66FFA5FEDEAC__INCLUDED_)
#define AFX_OPTIONSDLG_H__3BBDC969_E1FB_4E59_96A9_66FFA5FEDEAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OptionsDlg dialog

class OptionsDlg : public CDialog
{
// Construction
public:
	OptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsDlg)
	enum { IDD = IDD_OPTIONS_DIALOG };
	CStatic	m_HostsStaticCtrl;
	int		m_SinglePlayerServerOption;
	BOOL	m_UpdateHosts;
	BOOL	m_UpdateRgRegdata;
	BOOL	m_UpdateAuth;
	BOOL	m_UsePatchedAuthLoginDLL;
	short	m_Port;
	BOOL	m_UseSSL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSinglePlayerNet7Server();
	afx_msg void OnSinglePlayerJavaServer();
	afx_msg void OnNoneOfTheAbove();
	afx_msg void OnUsePatchedAuthloginDll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    bool    m_OptionLaunchNet7Server;
    bool    m_OptionLaunchJavaServer;
    bool    m_OptionUpdateHostsFile;
    bool    m_OptionUpdateRgRegDataIniFile;
    bool    m_OptionAuthIniFile;
    bool    m_OptionUsePatchedAuthLoginDLL;
    short   m_OptionAuthLoginPort;
    bool    m_OptionAuthLoginSSL;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__3BBDC969_E1FB_4E59_96A9_66FFA5FEDEAC__INCLUDED_)
