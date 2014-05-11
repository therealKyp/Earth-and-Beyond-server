// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "launchnet7.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsDlg dialog


OptionsDlg::OptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(OptionsDlg)
	m_SinglePlayerServerOption = 0;
	m_UpdateHosts = FALSE;
	m_UpdateRgRegdata = FALSE;
	m_UpdateAuth = FALSE;
	m_UsePatchedAuthLoginDLL = FALSE;
	m_Port = 443;
	m_UseSSL = FALSE;
	//}}AFX_DATA_INIT
}


void OptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsDlg)
	DDX_Control(pDX, IDC_HOSTS_STATIC, m_HostsStaticCtrl);
	DDX_Radio(pDX, IDC_SINGLE_PLAYER_NET7_SERVER, m_SinglePlayerServerOption);
	DDX_Check(pDX, IDC_UPDATE_HOSTS, m_UpdateHosts);
	DDX_Check(pDX, IDC_UPDATE_RG_REGDATA, m_UpdateRgRegdata);
	DDX_Check(pDX, IDC_UPDATE_AUTH, m_UpdateAuth);
	DDX_Check(pDX, IDC_USE_PATCHED_AUTHLOGIN_DLL, m_UsePatchedAuthLoginDLL);
	DDX_Text(pDX, IDC_PORT, m_Port);
	DDV_MinMaxInt(pDX, m_Port, 80, 32767);
	DDX_Check(pDX, IDC_USE_SSL, m_UseSSL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsDlg, CDialog)
	//{{AFX_MSG_MAP(OptionsDlg)
	ON_BN_CLICKED(IDC_SINGLE_PLAYER_NET7_SERVER, OnSinglePlayerNet7Server)
	ON_BN_CLICKED(IDC_SINGLE_PLAYER_JAVA_SERVER, OnSinglePlayerJavaServer)
	ON_BN_CLICKED(IDC_NONE_OF_THE_ABOVE, OnNoneOfTheAbove)
	ON_BN_CLICKED(IDC_USE_PATCHED_AUTHLOGIN_DLL, OnUsePatchedAuthloginDll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsDlg message handlers

BOOL OptionsDlg::OnInitDialog() 
{
    char *hosts;
    if (m_OptionLaunchNet7Server)
    {
        m_SinglePlayerServerOption = 0;
        hosts = "127.0.0.1  local.net-7.org";
    }
    else if (m_OptionLaunchJavaServer)
    {
        m_SinglePlayerServerOption = 1;
        hosts = "127.0.0.1  auth.enbserver.com";
    }
    else
    {
        m_SinglePlayerServerOption = 2;
        hosts = "";
    }

    m_UpdateHosts               = m_OptionUpdateHostsFile           ? 1 : 0;
    m_UpdateRgRegdata           = m_OptionUpdateRgRegDataIniFile    ? 1 : 0;
    m_UpdateAuth                = m_OptionAuthIniFile               ? 1 : 0;
    m_UsePatchedAuthLoginDLL    = m_OptionUsePatchedAuthLoginDLL    ? 1 : 0;
    m_UseSSL                    = m_OptionAuthLoginSSL              ? 1 : 0;
    m_Port                      = m_OptionAuthLoginPort;

    CDialog::OnInitDialog();

    m_HostsStaticCtrl.SetWindowText(hosts);

    GetDlgItem(IDC_UPDATE_HOSTS)->EnableWindow(m_SinglePlayerServerOption != 2);

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void OptionsDlg::OnOK() 
{
    UpdateData(TRUE);

    m_OptionLaunchNet7Server        = (m_SinglePlayerServerOption == 0);
    m_OptionLaunchJavaServer        = (m_SinglePlayerServerOption == 1);
    m_OptionUpdateHostsFile         = (m_UpdateHosts != 0);
    m_OptionUpdateRgRegDataIniFile  = (m_UpdateRgRegdata != 0);
    m_OptionAuthIniFile             = (m_UpdateAuth != 0);
    m_OptionUsePatchedAuthLoginDLL  = (m_UsePatchedAuthLoginDLL != 0);
    m_OptionAuthLoginSSL            = (m_UseSSL != 0);
    m_OptionAuthLoginPort           = m_Port;

	CDialog::OnOK();
}


void OptionsDlg::OnSinglePlayerNet7Server() 
{
    m_HostsStaticCtrl.SetWindowText("127.0.0.1  local.net-7.org");

    UpdateData(TRUE);
    m_SinglePlayerServerOption = 0;
    m_UpdateHosts = m_OptionUpdateHostsFile;
    m_Port = 443;   // local single-player
    m_UseSSL = true;
    m_UsePatchedAuthLoginDLL = true;
    UpdateData(FALSE);
    GetDlgItem(IDC_USE_SSL)->EnableWindow(m_UsePatchedAuthLoginDLL);
    GetDlgItem(IDC_PORT)->EnableWindow(m_UsePatchedAuthLoginDLL);

    GetDlgItem(IDC_UPDATE_HOSTS)->EnableWindow(true);
}

void OptionsDlg::OnSinglePlayerJavaServer() 
{
    m_HostsStaticCtrl.SetWindowText("127.0.0.1  account.ea.com");

    UpdateData(TRUE);
    m_SinglePlayerServerOption = 1;
    m_UpdateHosts = m_OptionUpdateHostsFile;
    m_Port = 80;
    m_UseSSL = false;
    m_UsePatchedAuthLoginDLL = true;
    UpdateData(FALSE);
    GetDlgItem(IDC_USE_SSL)->EnableWindow(m_UsePatchedAuthLoginDLL);
    GetDlgItem(IDC_PORT)->EnableWindow(m_UsePatchedAuthLoginDLL);

    GetDlgItem(IDC_UPDATE_HOSTS)->EnableWindow(true);
}

void OptionsDlg::OnNoneOfTheAbove() 
{
    m_HostsStaticCtrl.SetWindowText("");

    UpdateData(TRUE);
    m_OptionUpdateHostsFile = (m_UpdateHosts != 0);
    m_UpdateHosts = false;
    m_Port = 8891; // multi-player
    m_UseSSL = true;
    m_UsePatchedAuthLoginDLL = true;
    UpdateData(FALSE);
    GetDlgItem(IDC_USE_SSL)->EnableWindow(m_UsePatchedAuthLoginDLL);
    GetDlgItem(IDC_PORT)->EnableWindow(m_UsePatchedAuthLoginDLL);

    GetDlgItem(IDC_UPDATE_HOSTS)->EnableWindow(false);
}

void OptionsDlg::OnUsePatchedAuthloginDll() 
{
    UpdateData(TRUE);
    GetDlgItem(IDC_USE_SSL)->EnableWindow(m_UsePatchedAuthLoginDLL);
    GetDlgItem(IDC_PORT)->EnableWindow(m_UsePatchedAuthLoginDLL);
}
