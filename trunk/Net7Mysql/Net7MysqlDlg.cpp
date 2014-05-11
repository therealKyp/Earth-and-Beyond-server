// Net7MysqlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Net7Mysql.h"
#include "Net7MysqlDlg.h"
#include "Tab2.h"
#include "Tab3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <fstream.h>
#include <istream.h>
#include <string.h>


// Globals!
sql_connection_c m_MySQL_Connect;
bool		m_Connected;		// True if connected to mysql!

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNet7MysqlDlg dialog

CNet7MysqlDlg::CNet7MysqlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNet7MysqlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNet7MysqlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNet7MysqlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNet7MysqlDlg)
	DDX_Control(pDX, IDC_DATABASE, m_Mysql_Database);
	DDX_Control(pDX, IDC_SERVER, m_Mysql_Server);
	DDX_Control(pDX, IDC_USER, m_Mysql_User);
	DDX_Control(pDX, IDC_PASSWORD, m_Mysql_Password);
	//DDX_Control(pDX, IDC_DEBUG, m_Debug);
	DDX_Control(pDX, IDC_CONNECT, m_ConnectButtion);
	DDX_Control(pDX, IDC_SQL_FILE, m_MySQL_File);
	DDX_Control(pDX, IDC_TABS, m_Tabs);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNet7MysqlDlg, CDialog)
	//{{AFX_MSG_MAP(CNet7MysqlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_DUMP_SQL, OnDumpSql)
	ON_BN_CLICKED(IDC_DELETE_TABLE, OnDeleteTable)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABS, OnSelchangeTabs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNet7MysqlDlg message handlers

BOOL CNet7MysqlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// ------- Set up Tabs with Classes ---------
	m_tabPages[0] = new Tab2;
	m_tabPages[0]->Create(IDD_TAB1, this);
	m_tabText[0] = "Users";

	m_tabPages[1] = new Tab3;
	m_tabPages[1]->Create(IDD_TAB3, this);
	m_tabText[1] = "StarBase";

	m_tabPages[2] = new ItemBase;
	m_tabPages[2]->Create(IDD_ITEMS, this);
	m_tabText[2] = "ItemBase";

	m_nNumberOfPages = 3;
	m_tabCurrent = 0;
	// ---------- Start up Variables -------------


	m_Mysql_Server.SetWindowText(STARTUP_SERVER);
	m_Mysql_Server.AddString(STARTUP_SERVER);
	m_Mysql_Server.AddString("localhost:3306");
	m_Mysql_Database.SetWindowText("net7");


	ReadServerFile(SERVER_FILE);

	m_Connected = false;

	// ----------- Load Tabs ---------------------

	CRect rcTabCtrl;
	m_Tabs.GetWindowRect(rcTabCtrl);
	m_Tabs.AdjustRect(FALSE, rcTabCtrl);
	ScreenToClient(rcTabCtrl);

	CSize sizeMax(0, 0);

	
	for(int tab=0;tab<m_nNumberOfPages;tab++)
	{
		m_Tabs.InsertItem(tab, m_tabText[tab]);

		CRect rcChildDlg;
		m_tabPages[tab]->GetWindowRect(rcChildDlg);
		int x = (rcTabCtrl.left+rcTabCtrl.right)/2 - (rcChildDlg.Width())/2;
		int y = (rcTabCtrl.top+rcTabCtrl.bottom)/2 - (rcChildDlg.Height())/2;
		m_tabPages[tab]->SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_HIDEWINDOW);
	}
	

	m_Tabs.SetCurSel(m_tabCurrent);

	UpdateTabs();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNet7MysqlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNet7MysqlDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNet7MysqlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNet7MysqlDlg::OnConnect() 
{
	char Hostname[128], User[128], Pass[128], Database[128];


	m_Mysql_Server.GetWindowText(Hostname, sizeof(Hostname));
	m_Mysql_User.GetWindowText(User, sizeof(User));
	m_Mysql_Password.GetWindowText(Pass, sizeof(Pass));
	m_Mysql_Database.GetWindowText(Database, sizeof(Database));

	m_MySQL_Connect.~sql_connection_c();
	m_MySQL_Connect.connect(Database, Hostname, User, Pass);

	if (m_MySQL_Connect.errc != 0)
	{
		if (m_MySQL_Connect.errc == 3) {		// Create database
			sql_query_c AccountLists ( &m_MySQL_Connect );
			char query[1024];
			// Create & Use database
			sprintf(query, "CREATE DATABASE %s;", Database);
			AccountLists.execute( query );

			sprintf(query, "USE %s;", Database);
			AccountLists.execute( query );
		} else 	{
			MessageBox("Error connecting to MySQL!", "Net-7 MySQL", MB_ICONERROR);
			m_Connected = false;
			return;
		}
	}

	// -- See if host is in the list allready
	bool Found = false;
	CString Hosts;

	for(int id=0;id<m_Mysql_Server.GetCount();id++)
	{
		m_Mysql_Server.GetLBText(id, Hosts);

		if (Hosts.Compare(Hostname) == 0)		// Found Host!
		{
			Found = true;
			break;
		}
	}

	if (!Found)
	{
		m_Mysql_Server.AddString(Hostname);
		AppendToServerFile(SERVER_FILE, Hostname);
	}

	m_Connected = true;
	m_ConnectButtion.EnableWindow(false);
	m_Mysql_Server.EnableWindow(false);
	m_Mysql_User.EnableWindow(false);
	m_Mysql_Password.EnableWindow(false);
	m_Mysql_Database.EnableWindow(false);
}

void CNet7MysqlDlg::OnBrowse() 
{
	CFileDialog open_dlg(TRUE,NULL,"*.sql",OFN_OVERWRITEPROMPT,"sql file (*.sql)|*.sql||");
    int iRet = open_dlg.DoModal();
    CString file_name;
    file_name = open_dlg.GetPathName();

    if(iRet == IDOK)
	{
		m_MySQL_File.SetWindowText(file_name);
	}	
}

void CNet7MysqlDlg::OnDumpSql() 
{

	char file_name[256];

	m_MySQL_File.GetWindowText(file_name, sizeof(file_name));

	if (file_name[0] == 0)
	{
		MessageBox("Must specify a filename!", "Net-7 MySQL", MB_ICONERROR);
		return;
	}

	if (!m_Connected)
	{
		MessageBox("Must connect to MySQL!", "Net-7 MySQL", MB_ICONERROR);
		return;
	}

	ifstream in;
	char line[2024];
	CString hugeline;
	CString debug;

	in.open( file_name, ifstream::in);

	if ( in.good()  ) {


		while ( in.getline(line, sizeof(line) ) ) 
		{
			if ( strlen(line) > 2 && line[0] != '-' && line[1] != '-' && line[0] != '/' && line[1] != '*')	// no coment
			{
				hugeline += line;
			}
		}
		in.close();

		sql_query_c AccountLists ( &m_MySQL_Connect );

		char * ParsedLine = strtok((char *)(LPCTSTR)hugeline, ";");
		while(ParsedLine)
		{
			ParsedLine = strtok(NULL, ";");

			if (ParsedLine && strlen(ParsedLine) > 3)
			{
				AccountLists.execute( ParsedLine );

				debug += ParsedLine;
				debug += ";\r\nline: ";
			}
		}
	}


}

void CNet7MysqlDlg::OnDeleteTable() 
{
	if (!m_Connected)
	{
		MessageBox("Must connect to MySQL!", "Net-7 MySQL", MB_ICONERROR);
		return;
	}

	if (MessageBox("NOTICE! THIS DELETES ALL DATA!\r\nAre you sure?", "Net-7 MySQL", MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
	{
		return;
	}


	char Database[128];

	m_Mysql_Database.GetWindowText(Database, sizeof(Database));

	sql_query_c AccountLists ( &m_MySQL_Connect );

	char query[1024];

	// Drop & Create & Use database
	sprintf(query, "DROP DATABASE %s;", Database);
	AccountLists.execute( query );
	sprintf(query, "CREATE DATABASE %s;", Database);
	AccountLists.execute( query );
	sprintf(query, "USE %s;", Database);
	AccountLists.execute( query );
	
}

void CNet7MysqlDlg::UpdateTabs()
{
	m_tabCurrent = m_Tabs.GetCurSel();

	for(int tab=0;tab<m_nNumberOfPages;tab++)
	{
		if (tab != m_tabCurrent)
			m_tabPages[tab]->ShowWindow(SW_HIDE);
		else
			m_tabPages[tab]->ShowWindow(SW_SHOW);
	}
	
	
}

void CNet7MysqlDlg::OnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	*pResult = 0;

	UpdateTabs();
}

void CNet7MysqlDlg::AppendToServerFile(char *file, char *hostname)
{
	//adds a hostname to a serverfile
	char buffer[MAX_PATH];
	sprintf(buffer, "%s",hostname);;

	FILE *f = fopen(file, "a");
	if (f)
	{
		fprintf(f, "%s\n", hostname);
		fclose(f);
	}
}

void CNet7MysqlDlg::ReadServerFile(char *file)
{
	//reads all hostnames from the serverfile
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
					m_Mysql_Server.AddString(buffer);
			}
		}
		fclose(f);
	}
}
