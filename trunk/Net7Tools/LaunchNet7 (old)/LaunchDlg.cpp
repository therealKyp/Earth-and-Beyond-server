// LaunchDlg.cpp

#include "stdafx.h"
#include "LaunchNet7.h"
#include "LaunchDlg.h"
#include <winsock2.h>
#include <tlhelp32.h>

//registration keys
#define EB_REGKEY		        "SOFTWARE\\Westwood Studios\\Earth and Beyond\\Registration\\"
#define NET7_LAUNCHER_REGKEY	"SOFTWARE\\Westwood Studios\\Earth and Beyond\\LaunchNet7\\"

/* File names */
#define NET7_SERVER_FILE		"Net7Servers.txt"
#define JAVA_SERVER_FILE		"JavaServers.txt"
#define	NET7_LOCAL_CFG			"Net7Config.cfg"
#define LAUNCHER_CONFIG			"LauncherConfig.cfg"

//#define NET7_EXE_PATH			"Net7.exe"
#define NET7_EXE_PATH			"Net7Proxy.exe"
#define SOCKET_TEST_PATH		"sockettest.exe"
#define NET7_PROXY              "Net7Proxy.exe"
#define DETOURS_PATH			"Detours.exe"

#define NET7_SP_DOMAIN	        "local.net-7.org"

/* Buffer for the path from which the launcher is run and the overall root*/
char APP_PATH[MAX_PATH];
char ROOT_PATH[MAX_PATH];

//paths that are dependant on ENB location
char RELEASE_PATH[MAX_PATH] =			"C:\\Program Files\\EA GAMES\\Earth & Beyond\\release";
char INI_PATH[MAX_PATH] =				"C:\\Program Files\\EA GAMES\\Earth & Beyond\\Data\\client\\ini";
char NETWORK_INI_PATH[MAX_PATH] =		"C:\\Program Files\\EA GAMES\\Earth & Beyond\\Data\\common";
char AUTHLOGIN_FILE[MAX_PATH] =			"C:\\Program Files\\EA GAMES\\Earth & Beyond\\release\\authlogin.dll";
char ENB_ROOT_PATH[MAX_PATH] =			"C:\\Program Files\\EA GAMES\\Earth & Beyond\\";

typedef enum
{
	Net7SP, JavaMP, Net7MP
} Servers;

/* Begin CFG Variables */
char CLIENT_PATH[MAX_PATH] = "C:\\Program Files\\EA GAMES\\Earth & Beyond\\release\\client.exe";
int Net7_SP_PORT = 443;
int Net7_SP_HTTPS = 1;
int Net7_SP_PROTOCOL = 0;
int Java_MP_PORT = 443;
int Java_MP_HTTPS = 1;
int Java_MP_PROTOCOL = 0;
int Net7_MP_PORT = 443;
int Net7_MP_HTTPS = 1;
int Net7_MP_PROTOCOL = 0;
Servers Default_Server = Net7SP;
char Default_JavaMP_Host[MAX_PATH] = "ebemu.homelinux.org";
char Default_Net7MP_Host[MAX_PATH] = "play.net-7.org";
int Client_Detours = 0;
/* End CFG Variables */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LaunchDlg dialog

LaunchDlg::LaunchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LaunchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(LaunchDlg)
	m_AuthPortValue = 0;
	m_AuthHttpsValue = FALSE;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Winsock startup
	WSADATA	wsaData = {NULL};
	WSAStartup(MAKEWORD(2, 0), &wsaData);
}

void LaunchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LaunchDlg)
	DDX_Control(pDX, IDC_CLIENT_DETOURS, m_ClientDetours);
	DDX_Control(pDX, IDC_SERVER_NAME, m_ServerNameComboBox);
	DDX_Control(pDX, IDC_HTTPS, m_AuthHttpsCtrl);
	DDX_Control(pDX, IDC_AUTH_PORT, m_AuthPortCtrl);
	DDX_Control(pDX, IDC_BROWSE, m_BrowseCtrl);
	DDX_Control(pDX, IDC_CLIENT_PATH, m_ClientPathCtrl);
	DDX_Control(pDX, IDC_OK, m_PlayButton);
	DDX_Control(pDX, IDC_NET7_SP, m_Net7SPCtrl);
	DDX_Control(pDX, IDC_NET7_MP, m_Net7MPCtrl);
	DDX_Control(pDX, IDC_JAVA_MP, m_JavaMPCtrl);
	DDX_Control(pDX, IDC_TEST, m_TestButton);
	DDX_Control(pDX, IDC_SERVER_STATUS, m_ServerStatusCtrl);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_ProgressCtrl);
	DDX_Control(pDX, IDC_STATUS, m_StatusCtrl);
	DDX_Text(pDX, IDC_AUTH_PORT, m_AuthPortValue);
	DDV_MinMaxInt(pDX, m_AuthPortValue, 0, 32000);
	DDX_Check(pDX, IDC_HTTPS, m_AuthHttpsValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(LaunchDlg, CDialog)
	//{{AFX_MSG_MAP(LaunchDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_EDITCHANGE(IDC_SERVER_NAME, OnEditchangeServerName)
	ON_CBN_SELCHANGE(IDC_SERVER_NAME, OnSelchangeServerName)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_JAVA_MP, OnJavaMp)
	ON_BN_CLICKED(IDC_NET7_MP, OnNet7Mp)
	ON_BN_CLICKED(IDC_NET7_SP, OnNet7Sp)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_HTTPS, OnHttps)
	ON_EN_KILLFOCUS(IDC_AUTH_PORT, OnKillfocusAuthPort)
	ON_BN_CLICKED(IDC_CLIENT_DETOURS, OnClientDetours)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LaunchDlg message handlers


BOOL LaunchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	/* Obtain the application path */
	GetCurrentDirectory(sizeof(APP_PATH),APP_PATH);

	/* Obtain the root path to Net7 installation */
	char * pch = strstr(APP_PATH,"\\bin");
	if (pch)
	{
		memcpy(ROOT_PATH, APP_PATH, pch - APP_PATH);
	}
	else
	{
		memset(ROOT_PATH, 0, sizeof(ROOT_PATH));
	}

	m_StatusCtrl.SetWindowText("Validating Earth and Beyond installation...");

	m_ValidationTimer = 0;

	m_ServerStatusCtrl.SetWindowText("");
	m_BrowseCtrl.EnableWindow(false);
	m_PlayButton.EnableWindow(false);
	m_TestButton.EnableWindow(false);
	m_Net7SPCtrl.EnableWindow(false);
	m_Net7MPCtrl.EnableWindow(false);
	m_JavaMPCtrl.EnableWindow(false);
	m_AuthHttpsCtrl.EnableWindow(false);
	m_AuthPortCtrl.EnableWindow(false);
	m_ServerNameComboBox.EnableWindow(false);
	m_ClientDetours.EnableWindow(false);

	Sleep(100);
	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_ProgressCtrl.SetRange(0,20);
	m_ProgressCtrl.SetPos(0);

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void LaunchDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

HCURSOR LaunchDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void LaunchDlg::OnNet7Sp() 
{
	Default_Server = Net7SP;
	m_Net7MPCtrl.SetCheck(false);
	m_JavaMPCtrl.SetCheck(false);
	m_TestButton.EnableWindow(false);
	m_PlayButton.EnableWindow(true);

	SetAuthPort(Net7_SP_PORT);
	m_AuthHttpsCtrl.SetCheck(Net7_SP_HTTPS);

	m_ServerNameComboBox.ResetContent();
	m_ServerNameComboBox.SetWindowText(NET7_SP_DOMAIN);
	m_ServerNameComboBox.EnableWindow(false);
	m_ServerStatusCtrl.SetWindowText("PLAY");
}

void LaunchDlg::OnNet7Mp() 
{
	Default_Server = Net7MP;
	m_Net7SPCtrl.SetCheck(false);
	m_JavaMPCtrl.SetCheck(false);
	m_ServerNameComboBox.EnableWindow(true);
	m_TestButton.EnableWindow(true);
	m_PlayButton.EnableWindow(false);

	SetAuthPort(Net7_MP_PORT);
	m_AuthHttpsCtrl.SetCheck(Net7_MP_HTTPS);

	m_ServerNameComboBox.ResetContent();
	ReadServerFile(NET7_SERVER_FILE);
	m_ServerNameComboBox.SetWindowText(Default_Net7MP_Host);
	OnSelchangeServerName();	
}


void LaunchDlg::OnJavaMp() 
{
	Default_Server = JavaMP;
	m_Net7SPCtrl.SetCheck(false);
	m_Net7MPCtrl.SetCheck(false);
	m_ServerNameComboBox.EnableWindow(true);
	m_TestButton.EnableWindow(true);
	m_PlayButton.EnableWindow(false);

	SetAuthPort(Java_MP_PORT);
	m_AuthHttpsCtrl.SetCheck(Java_MP_HTTPS);

	m_ServerNameComboBox.ResetContent();
	ReadServerFile(JAVA_SERVER_FILE);
	m_ServerNameComboBox.SetWindowText(Default_JavaMP_Host);
	OnSelchangeServerName();	
}

void LaunchDlg::SetAuthPort(int port)
{
	char buff[10];

	itoa(port, buff, 10);
	m_AuthPortCtrl.SetWindowText(buff);
}

void LaunchDlg::OnEditchangeServerName() 
{
	m_ServerStatusCtrl.SetWindowText("");
	m_PlayButton.EnableWindow(false);
	
	CString hostname;
	m_ServerNameComboBox.GetWindowText(hostname);
	m_TestButton.EnableWindow(!hostname.IsEmpty());
}

void LaunchDlg::OnSelchangeServerName()
{
	m_ServerStatusCtrl.SetWindowText("");
	m_PlayButton.EnableWindow(false);

	CString hostname;
	int cursel = m_ServerNameComboBox.GetCurSel();

	if (cursel != LB_ERR)
	{
		m_ServerNameComboBox.GetLBText(cursel, hostname);
	}
	else
	{
		m_ServerNameComboBox.GetWindowText(hostname);
	}

	m_TestButton.EnableWindow(!hostname.IsEmpty());
}

//this pings the selected authentication port on the selected domain
void LaunchDlg::OnTest() 
{
	bool online = false;
	char hostname[128];
	char msg[1024];
	
	UpdateData(TRUE);
	m_ServerStatusCtrl.SetWindowText("PINGING");
	m_ServerNameComboBox.GetWindowText(hostname, sizeof(hostname));
	struct hostent * host = gethostbyname(hostname);
	if (host)
	{
		long ip_long = *((long *) host->h_addr);
		if (ip_long == 0)
		{
			sprintf(msg, "Unable to resolve IP address for '%s'", hostname);
			MessageBox(msg, "LaunchNet7", MB_ICONERROR);
		}
		else
		{
			SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (s == INVALID_SOCKET)
			{
				MessageBox("Unable to open TCP socket", "LaunchNet7", MB_ICONERROR);
			}
			else
			{
				sockaddr_in addr = {NULL};
				addr.sin_addr.S_un.S_addr = ip_long;
				addr.sin_family = AF_INET;
				addr.sin_port = htons(m_AuthPortValue);

				//if the socket connected on the port, then the server is online
				if (connect(s, (sockaddr *) &addr, sizeof(addr)) == 0)
					online = true;                    

				closesocket(s);
			}
		}
	}

	if (online)
	{
		m_ServerStatusCtrl.SetWindowText("ONLINE");
		m_PlayButton.EnableWindow(true);

		//cycle through combobox list for hostname
		bool found = false;
		int count = m_ServerNameComboBox.GetCount();
		CString str_combo;
		CString str_host(hostname);
		for (int i = 0; i < count; i++)
		{
			m_ServerNameComboBox.GetLBText(i, str_combo);
			if (str_host == str_combo)
			{
				found = true;
				break;
			}
		}

		//add hostname if not found
		if (!found)
		{
 			m_ServerNameComboBox.AddString(hostname);

			if (m_JavaMPCtrl.GetCheck() == BST_CHECKED)
				AppendToServerFile(JAVA_SERVER_FILE, hostname);

			if (m_Net7MPCtrl.GetCheck() == BST_CHECKED)
				AppendToServerFile(NET7_SERVER_FILE, hostname);
		}
	}
	else
	{
		m_ServerStatusCtrl.SetWindowText("OFFLINE");
		m_PlayButton.EnableWindow(false);
	}
}

void LaunchDlg::AppendToServerFile(char *file, char *hostname)
{
	//adds a hostname to a serverfile
	char buffer[MAX_PATH];
	sprintf(buffer, "%s",hostname);;

	SetCurrentDirectory(APP_PATH);
	FILE *f = fopen(file, "a");
	if (f)
	{
		fprintf(f, "%s\n", hostname);
		fclose(f);
	}
}

void LaunchDlg::SaveConfig()
{
	SetCurrentDirectory(APP_PATH);
	FILE *f = fopen(LAUNCHER_CONFIG, "w");
	if (f)
	{
		char buffer[MAX_PATH];
		sprintf(buffer, "clientpath=%s\n", CLIENT_PATH);
		fprintf(f, buffer);

		sprintf(buffer, "net7spport=%d\n", Net7_SP_PORT);
		fprintf(f, buffer);

		sprintf(buffer, "net7sphttps=%d\n", Net7_SP_HTTPS);
		fprintf(f, buffer);

		sprintf(buffer, "net7spprotocol=%d\n", Net7_SP_PROTOCOL);
		fprintf(f, buffer);

        sprintf(buffer, "javampport=%d\n", Java_MP_PORT);
		fprintf(f, buffer);

		sprintf(buffer, "javamphttps=%d\n", Java_MP_HTTPS);
		fprintf(f, buffer);

		sprintf(buffer, "javampprotocol=%d\n", Java_MP_PROTOCOL);
		fprintf(f, buffer);

		sprintf(buffer, "net7mpport=%d\n", Net7_MP_PORT);
		fprintf(f, buffer);

		sprintf(buffer, "net7mphttps=%d\n", Net7_MP_HTTPS);
		fprintf(f, buffer);

		sprintf(buffer, "net7mpprotocol=%d\n", Net7_MP_PROTOCOL);
		fprintf(f, buffer);

		sprintf(buffer, "defaultserver=%d\n", Default_Server);
		fprintf(f, buffer);

		sprintf(buffer, "defaultnet7mphost=%s\n", Default_Net7MP_Host);
		fprintf(f, buffer);

		sprintf(buffer, "defaultjavamphost=%s\n", Default_JavaMP_Host);
		fprintf(f, buffer);

		sprintf(buffer, "clientdetours=%d\n", Client_Detours);
		fprintf(f, buffer);


		fclose(f);
	}
}
void LaunchDlg::LoadConfig()
{
	bool success = false;

	SetCurrentDirectory(APP_PATH);
	FILE *f = fopen(LAUNCHER_CONFIG, "r");
	if (f)
	{
		char buffer[MAX_PATH];
		char *cmd;
		char *data;

		while (!feof(f))
		{
			if (fgets(buffer, sizeof(buffer), f))
			{
				cmd = strtok(buffer, "=");
				data = strtok(NULL, "\n\r");

				if (strcmp(cmd, "clientpath")==0)
				{
					strcpy(CLIENT_PATH, data);
				}
				else if (strcmp(cmd, "net7spport")==0)
				{
					Net7_SP_PORT = atoi(data);
				}
				else if (strcmp(cmd, "net7sphttps")==0)
				{
					Net7_SP_HTTPS = atoi(data);
				}
				else if (strcmp(cmd, "net7spprotocol")==0)
				{
					Net7_SP_PROTOCOL = atoi(data);
				}
				else if (strcmp(cmd, "javampport")==0)
				{
					Java_MP_PORT = atoi(data);
				}
				else if (strcmp(cmd, "javamphttps")==0)
				{
					Java_MP_HTTPS = atoi(data);
				}
				else if (strcmp(cmd, "javampprotocol")==0)
				{
					Java_MP_PROTOCOL = atoi(data);
				}
				else if (strcmp(cmd, "net7mpport")==0)
				{
					Net7_MP_PORT = atoi(data);
				}
				else if (strcmp(cmd, "net7mphttps")==0)
				{
					Net7_MP_HTTPS = atoi(data);
				}
				else if (strcmp(cmd, "net7mpprotocol")==0)
				{
					Net7_MP_PROTOCOL = atoi(data);
				}
				else if (strcmp(cmd, "defaultserver")==0)
				{
					Default_Server = (Servers) atoi(data);
				}
				else if (strcmp(cmd, "defaultnet7mphost")==0)
				{
					strcpy(Default_Net7MP_Host, data);
				}
				else if (strcmp(cmd, "defaultjavamphost")==0)
				{
					strcpy(Default_JavaMP_Host, data);
				}
				else if (strcmp(cmd, "clientdetours")==0)
				{
					Client_Detours = atoi(data);
				}
			}
		}
		fclose(f);
	}

	UpdateData(TRUE);
	m_ClientPathCtrl.SetWindowText(CLIENT_PATH);
	GenerateENBPaths();

	m_ClientDetours.EnableWindow(true);
	m_ClientDetours.SetCheck(Client_Detours);


	m_PlayButton.EnableWindow(true);
	m_BrowseCtrl.EnableWindow(true);
	m_Net7SPCtrl.EnableWindow(true);
	m_Net7MPCtrl.EnableWindow(true);
	m_JavaMPCtrl.EnableWindow(true);
	m_AuthHttpsCtrl.EnableWindow(true);
	m_AuthPortCtrl.EnableWindow(true);

	m_Net7SPCtrl.SetCheck(false);
	m_Net7MPCtrl.SetCheck(false);
	m_JavaMPCtrl.SetCheck(false);


	switch (Default_Server)
	{
	case Net7SP:
		m_Net7SPCtrl.SetCheck(true);
		SetAuthPort(Net7_SP_PORT);
		m_AuthHttpsCtrl.SetCheck(Net7_SP_HTTPS);
		m_PlayButton.EnableWindow(true);

		m_ServerNameComboBox.ResetContent();
		m_ServerNameComboBox.SetWindowText(NET7_SP_DOMAIN);
		m_ServerStatusCtrl.SetWindowText("PLAY");
		break;

	case Net7MP:
		m_Net7MPCtrl.SetCheck(true);
		SetAuthPort(Net7_MP_PORT);
		m_AuthHttpsCtrl.SetCheck(Net7_MP_HTTPS);

		m_ServerNameComboBox.ResetContent();
		ReadServerFile(NET7_SERVER_FILE);
		m_ServerNameComboBox.SetWindowText(Default_Net7MP_Host);
		OnSelchangeServerName();	
		m_ServerNameComboBox.EnableWindow(true);
		break;

	case JavaMP:
		m_JavaMPCtrl.SetCheck(true);
		SetAuthPort(Java_MP_PORT);
		m_AuthHttpsCtrl.SetCheck(Java_MP_HTTPS);

		m_ServerNameComboBox.ResetContent();
		ReadServerFile(JAVA_SERVER_FILE);
		m_ServerNameComboBox.SetWindowText(Default_JavaMP_Host);
		OnSelchangeServerName();	
		m_ServerNameComboBox.EnableWindow(true);
		break;
	}
}

void LaunchDlg::ReadServerFile(char *file)
{
	//reads all hostnames from the serverfile
	SetCurrentDirectory(APP_PATH);
	FILE *f = fopen(file, "r");
	if (f)
	{
		char buffer[MAX_PATH];
		while (!feof(f))
		{
			if (fgets(buffer, sizeof(buffer), f))
			{
				strtok(buffer, " \r\n\t");

				if (buffer[0])
					m_ServerNameComboBox.AddString(buffer);
			}
		}
		fclose(f);
	}
}

void LaunchDlg::CreateSQLCFG()
{
	SetCurrentDirectory(APP_PATH);
	FILE *f = fopen(NET7_LOCAL_CFG, "w");
	if (f)
	{
		fprintf(f, "domain=local.net-7.org\n");
		fprintf(f, "mysql_user=enb\n");
		fprintf(f, "mysql_pass=enbserver\n");
		fprintf(f, "mysql_host=localhost:3307\n");
		fprintf(f, "mysql_db=net7\n");
		fclose(f);
	}
}

void LaunchDlg::OnOk() 
{
	//stores the values of the AUTH controls in their variables
	UpdateData(TRUE);

	UpdateRegistry();

	if(!PatchAuthDLL())
		return;

    CheckRG_RegData();

	if(!UpdateFile(INI_PATH, "rg_regdata.ini"))
		return;
	if(!UpdateFile(INI_PATH, "auth.ini"))
		return;
	if(!UpdateFile(NETWORK_INI_PATH, "network.ini"))
		return;

	SaveConfig();

	switch (Default_Server)
	{
	case Net7SP:

		ShowWindow(SW_MINIMIZE);
		LaunchNet7Server();
        
		Sleep(6000);

        LaunchProxy();

		break;

	case Net7MP:
		ShowWindow(SW_MINIMIZE);

        LaunchProxy();

		break;

	case JavaMP:
		ShowWindow(SW_MINIMIZE);
		LaunchClient();
		break;
	}

	CDialog::OnOK();
}

void LaunchDlg::LaunchNet7Server()
{
	char cmd[MAX_PATH];
	STARTUPINFO si = {NULL};
	PROCESS_INFORMATION pi = {NULL};

	sprintf(cmd, "Net7.exe");

	SetCurrentDirectory(APP_PATH);
	CreateProcess(NET7_EXE_PATH, cmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);
}

void LaunchDlg::LaunchSocketTest()
{
	// Determine the correct domain name
	char domain[MAX_PATH];
	m_ServerNameComboBox.GetWindowText(domain, sizeof(domain));
	
	struct hostent *list = gethostbyname(domain);
	if (list)
	{
		char cmd[MAX_PATH];
		char short_path[MAX_PATH];

		/* Get short path name inorder to send via DOS */
		GetShortPathName(CLIENT_PATH, short_path, MAX_PATH);

		unsigned char * ip = (unsigned char *) list->h_addr_list[0];
		sprintf(cmd, "SocketTest.exe /ADDR:%d.%d.%d.%d /CLIENT:%s", ip[0], ip[1], ip[2], ip[3], short_path);

		if (Client_Detours)
		{
			strcat(cmd, " /L");
		}


		STARTUPINFO si = {NULL};
		PROCESS_INFORMATION pi = {NULL};

		SetCurrentDirectory(APP_PATH);
		CreateProcess(SOCKET_TEST_PATH, cmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);
	}
	else
	{
		MessageBox("Unable to resolve IP address", "LaunchNet7");
	}
}

void LaunchDlg::LaunchProxy()
{
	// Determine the correct domain name

	/*char cmd[MAX_PATH];
	STARTUPINFO si = {NULL};
	PROCESS_INFORMATION pi = {NULL};

	sprintf(cmd, "Net7Proxy.exe");

	SetCurrentDirectory("c:\net7\bin");
	CreateProcess(NET7_PROXY, cmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);*/

#if 1
	char domain[MAX_PATH];
	m_ServerNameComboBox.GetWindowText(domain, sizeof(domain));
	
	struct hostent *list = gethostbyname(domain);
	if (list)
	{
		char cmd[MAX_PATH];
		char short_path[MAX_PATH];

		/* Get short path name inorder to send via DOS */
		GetShortPathName(CLIENT_PATH, short_path, MAX_PATH);

		unsigned char * ip = (unsigned char *) list->h_addr_list[0];
		sprintf(cmd, "Net7Proxy.exe /ADDRESS:%d.%d.%d.%d /CLIENT:%s", ip[0], ip[1], ip[2], ip[3], short_path);

		if (Client_Detours)
		{
			strcat(cmd, " /L");
		}

		STARTUPINFO si = {NULL};
		PROCESS_INFORMATION pi = {NULL};

		SetCurrentDirectory(APP_PATH);
		CreateProcess(NET7_PROXY, cmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);
	}
	else
	{
		MessageBox("Unable to resolve IP address", "LaunchNet7");
	}
#endif
}

void LaunchDlg::LaunchClient()
{
	// Determine the correct domain name
	char cmd[128];
	char domain[MAX_PATH];
	m_ServerNameComboBox.GetWindowText(domain, sizeof(domain));
	
	struct hostent *list = gethostbyname(domain);
	if (!list)
	{
		MessageBox("Unable to resolve IP address", "LaunchNet7");
		return;
	}

	unsigned char * ip = (unsigned char *) list->h_addr_list[0];
	STARTUPINFO si = {NULL};
	PROCESS_INFORMATION pi = {NULL};

	if (Client_Detours)
	{
		char short_path[MAX_PATH];

		/* Get short path name inorder to send via DOS */
		GetShortPathName(CLIENT_PATH, short_path, MAX_PATH);

		sprintf(cmd, "Detours.exe /ADDR:%d.%d.%d.%d /CLIENT:%s",ip[0], ip[1], ip[2], ip[3], short_path);
        

		SetCurrentDirectory(APP_PATH);
		CreateProcess(DETOURS_PATH, cmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);
	}
	else
	{
		sprintf(cmd, "client.exe -SERVER_ADDR %d.%d.%d.%d -PROTOCOL TCP", ip[0], ip[1], ip[2], ip[3]);
        

		SetCurrentDirectory(RELEASE_PATH);
		CreateProcess(CLIENT_PATH, cmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);
	}
}

void LaunchDlg::UpdateRegistry()
{
	/* Open the Registry key */
	long status = ERROR_SUCCESS;
	REGSAM sam = KEY_READ + KEY_WRITE;
	HKEY hKey;
	status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, EB_REGKEY, 0, sam, &hKey);
	if (status != ERROR_SUCCESS)
	{
		DWORD disposition = 0;
		RegCreateKeyEx(HKEY_LOCAL_MACHINE, EB_REGKEY, 0, NULL, 0, sam, NULL, &hKey, &disposition );
	}

	/* Set the "Registered" flag in the system registry */
	unsigned long data = 1;

	RegSetValueEx(hKey, "Registered", 0, REG_DWORD, (unsigned char *) &data, sizeof(unsigned long));
	RegCloseKey(hKey);
}

void LaunchDlg::CheckRG_RegData()
{
	char src_filename[MAX_PATH];
	char dest_filename[MAX_PATH];

	//create full paths
	sprintf(src_filename, "%s\\rg_regdata_org", INI_PATH);
	sprintf(dest_filename, "%s\\rg_regdata.ini", INI_PATH);

	//check if we have the files
	bool have_src = CheckFile(src_filename);
	bool have_dest = CheckFile(dest_filename);

    if (!have_dest && have_src)
    {
        MoveFile(src_filename,dest_filename);
    }
}

bool LaunchDlg::UpdateFile(char *path, char *file)
{
	bool success = false;

	char src_filename[MAX_PATH];
	char dest_filename[MAX_PATH];
	char domain[MAX_PATH];
	char msg[1024];

	//connect to the domain thats in the combobox
	m_ServerNameComboBox.GetWindowText(domain, sizeof(domain));

	//create full paths
	sprintf(src_filename, "%s\\%s.orig", path, file);
	sprintf(dest_filename, "%s\\%s", path, file);

	//check if we have the files
	bool have_src = CheckFile(src_filename);
	bool have_dest = CheckFile(dest_filename);

	//we are missing the file we are trying to write 
	if (!have_dest)
	{		
		sprintf(msg, "%s is missing!", dest_filename);
		MessageBox(msg, "Net7 Launcher", MB_ICONERROR);
		return false;
	}

	//have both files, delete the backup and copy the source
	//this way we make a backup EVERY time
	if (have_src)
	{
		if (!DeleteFile(src_filename))
		{
			sprintf(msg, "Could not delete backup file \n%s", src_filename);
			MessageBox(msg, "Net7 Launcher", MB_ICONERROR);
			return false;
		}
	}

	//move file to <filename>.orig
	if (!MoveFile(dest_filename, src_filename))
	{
		sprintf(msg, "Could not create backup of \n%s", dest_filename);
		MessageBox(msg, "LaunchNet7", MB_ICONERROR);
		return false;
	}

	//copy the source file to the destination file one line at a time
	//add  \n to everyline that WE write
	//keep all data that we do not edit the same
	char buffer[1024];
	char *https;
	if (m_AuthHttpsValue)
		https = "https";
	else
		https = "http";

	FILE *src = fopen(src_filename, "r");
	FILE *dest = fopen(dest_filename, "w");

	if (src && dest)
	{
		while (!feof(src))
		{
			if (fgets(buffer, sizeof(buffer), src))
			{
				char *tmp = strtok(buffer,"\n\r");

				if (!tmp)
					memset(buffer, 0, sizeof(buffer));

				if (strncmp(buffer, "regserverurl=", strlen("regserverurl=")) == 0)
				{
					sprintf(buffer, "regserverurl=%s://%s/subsxml", https, domain);
					success = true;
				}
				else if (strncmp(buffer, "AAIUrl=", strlen("AAIUrl=")) == 0)
				{
					sprintf(buffer, "AAIUrl=%s", domain);
					success = true;
				}
				else if (strncmp(buffer, "LKeyUrl=", strlen("LKeyUrl=")) == 0)
				{
    				sprintf(buffer, "LKeyUrl=%s://%s/misc/touchsession.jsp?lkey=%%s", https, domain);
					success = true;
				}
				else if (strncmp(buffer, "Name", strlen("Name")) == 0 && (strcmp(file, "network.ini") == 0))
				{
					sprintf(buffer, "Name    = %s", domain);
					success = true;
				}

				fprintf(dest, "%s\n", buffer);

			}
		}

		fclose(src);
		fclose(dest);
	}

	if (!success)
	{
		sprintf(msg, "Failed to update %s", file);
		MessageBox(msg, "LaunchNet7", MB_ICONERROR);
	}

	return success;
}

bool LaunchDlg::PatchAuthDLL()
{
	//  HTTPS (SSL) = 'C0' or HTTP = '40'
	char url_flag = m_AuthHttpsValue ? 0xc0 : 0x40;
	short winsock_patch = short(0x0AEB);
	bool success = false;

	FILE *f = fopen(AUTHLOGIN_FILE, "rb+");
	if (f)
	{
		if (fseek(f, 0x82AD, SEEK_SET) == 0)	/* Port Location */
		{
			if (fwrite(&m_AuthPortValue, 1, 2, f) == 2)
			{
				if (fseek(f, 0x8328, SEEK_SET) == 0)	/* SSL Location */
				{
					if (fwrite(&url_flag, 1, 1, f) == 1)
					{
						if (fseek(f, 0x8292, SEEK_SET) == 0)	/* Winsock Timeout Location */
						{
							if (fwrite(&winsock_patch, 1, 2, f) == 2)
							{
								success = true;
								fclose(f);
							}
						}
					}
				}
			}
		}

		if (!success)
		{
			fclose(f);
			MessageBox("Unable to patch authlogin.dll", "LaunchNet7", MB_ICONERROR);
		}
	}
	else
	{
		MessageBox("Unable to open authlogin.dll", "LaunchNet7", MB_ICONERROR);
	}
	return (success);
}

// This validation step verifies that we have write access to the
// HKEY_LOCAL_MACHINE hive in the Windows registry.
bool LaunchDlg::ValidateProbeRegistry()
{
	bool success = true;
	// Open the Registry key
	long status = ERROR_SUCCESS;
	REGSAM sam = KEY_READ + KEY_WRITE;
	HKEY hKey = NULL;
	status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, NET7_LAUNCHER_REGKEY, 0, sam, &hKey);
	if (status != ERROR_SUCCESS)
	{
		DWORD disposition = 0;
		status = RegCreateKeyEx(HKEY_LOCAL_MACHINE, NET7_LAUNCHER_REGKEY, 0, NULL, 0, sam, NULL, &hKey, &disposition );
		if (status != ERROR_SUCCESS)
		{
			success = false;
		}
	}

	if (success)
	{
		// Set the "RegistryTest" flag in the system registry
		unsigned long data = 1;

		status = RegSetValueEx(hKey, "RegistryTest", 0, REG_DWORD, (unsigned char *) &data, sizeof(unsigned long));
		if (status != ERROR_SUCCESS)
		{
			success = false;
		}

		status = RegDeleteValue(hKey, "RegistryTest");
		if (status != ERROR_SUCCESS)
		{
			success = false;
		}
	}

	// Close the Registry key
	RegCloseKey(hKey);

	return (success);
}

void LaunchDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		ContinueValidation();
		m_ValidationTimer++;
		if (m_ValidationTimer > 20)
		{
			KillTimer(1);
			m_ProgressCtrl.SetPos(100);
			m_StatusCtrl.SetWindowText("Please select a server, test for connection, and hit Play");
			m_ServerStatusCtrl.SetWindowText("");

			LoadConfig();
		}
		else
		{
			m_ProgressCtrl.SetPos(m_ValidationTimer);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void LaunchDlg::ContinueValidation()
{
	char *message = NULL;

	switch (m_ValidationTimer)
	{
	case 5 :
		if (!ValidateProbeRegistry())
			message = "Earth & Beyond requires you to run with Administrative privileges!";
		break;
	}

	if (message)
		MessageBox(message, "LaunchNet7");

}

bool LaunchDlg::CheckFile(char *file) 
{
	bool present = false;
	FILE *f = fopen(file, "r");
	if (f)
	{
		present = true;
		fclose(f);
	}
	return (present);
}


void LaunchDlg::OnCancel() 
{
	CDialog::EndDialog(IDC_CANCEL);
	
}

void LaunchDlg::OnBrowse() 
{
	CFileDialog open_dlg(TRUE,NULL,"client.exe",OFN_OVERWRITEPROMPT,"E&B Client Exacutable (client.exe)|client.exe||");
	int iRet = open_dlg.DoModal();
	CString file_name;
	file_name = open_dlg.GetPathName();

	if(iRet == IDOK)
	{
		m_ClientPathCtrl.SetWindowText(file_name);
		strcpy(CLIENT_PATH, file_name);

		GenerateENBPaths();
		SaveConfig();
	}
}
//generates the ENB paths from the client path
void LaunchDlg::GenerateENBPaths() 
{
		int char_len;
		char buffer[MAX_PATH];

		strcpy(buffer, CLIENT_PATH);
		char_len = strlen(strstr(buffer,"release\\client.exe"));
		for (int i = 0; i < char_len; i++)
			buffer[strlen(buffer)-1] = '\0';
		strcpy(ENB_ROOT_PATH, buffer);
		sprintf(RELEASE_PATH, "%srelease",ENB_ROOT_PATH);
		sprintf(INI_PATH, "%sData\\client\\ini",ENB_ROOT_PATH);
		sprintf(NETWORK_INI_PATH, "%sData\\common",ENB_ROOT_PATH);
		sprintf(AUTHLOGIN_FILE, "%srelease\\authlogin.dll",ENB_ROOT_PATH);
}

void LaunchDlg::OnHttps() 
{
	UpdateData(TRUE);
	switch (Default_Server)
	{
	case Net7SP:
		Net7_SP_HTTPS = m_AuthHttpsValue;
		break;

	case Net7MP:
		Net7_MP_HTTPS = m_AuthHttpsValue;
		break;

	case JavaMP:
		Java_MP_HTTPS = m_AuthHttpsValue;
		break;
	}
}

void LaunchDlg::OnKillfocusAuthPort() 
{
	UpdateData(TRUE);
	switch (Default_Server)
	{
	case Net7SP:
		Net7_SP_PORT = m_AuthPortValue;
		break;

	case Net7MP:
		Net7_MP_PORT = m_AuthPortValue;
		break;

	case JavaMP:
		Java_MP_PORT = m_AuthPortValue;
		break;
	}
}

void LaunchDlg::OnClientDetours() 
{
	Client_Detours = m_ClientDetours.GetCheck();
}

