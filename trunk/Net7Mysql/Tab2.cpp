// Tab2.cpp : implementation file
//

#include "stdafx.h"
#include "Net7Mysql.h"
#include "Tab2.h"
#include "Net7MysqlDlg.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Tab2 dialog


Tab2::Tab2(CWnd* pParent /*=NULL*/)
	: CDialog(Tab2::IDD, pParent)
{
	
	//{{AFX_DATA_INIT(Tab2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Tab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Tab2)
	DDX_Control(pDX, IDC_USER, m_User);
	DDX_Control(pDX, IDC_ACCOUNT_TYPE, m_Type);
	DDX_Control(pDX, IDC_PASSWORD, m_Pass);
	DDX_Control(pDX, IDC_LOGIN, m_LastLogin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Tab2, CDialog)
	//{{AFX_MSG_MAP(Tab2)
	ON_BN_CLICKED(IDC_ADD_USER, OnAddUser)
	ON_CBN_SELCHANGE(IDC_USER, OnSelchangeUser)
	ON_BN_CLICKED(IDC_UPDATE_USERS, OnUpdateUsers)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL Tab2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Type.AddString("Admin");
	m_Type.AddString("Developer");
	m_Type.AddString("Head Game Master");
	m_Type.AddString("Deputy Game Master");
	m_Type.AddString("Server Game Master");
	m_Type.AddString("Beta Tester");
	m_Type.AddString("Player");
	m_Type.AddString("Donors");
	m_Type.AddString("Helper");
	m_Type.AddString("Baned");

	m_MaxAccounts = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// Tab2 message handlers

void Tab2::OnAddUser() 
{
	if (!m_Connected)
	{
		MessageBox("Must connect to MySQL!", "Net-7 MySQL", MB_ICONERROR);
		return;
	}

	// SetUp MySQL
	sql_query_c AccountLists ( &m_MySQL_Connect );
	char queryString[1024];


	// -- See if user is in the list allready
	bool Found = false;
	CString User;

	m_User.GetWindowText(User);

	for(int id=0;id<m_MaxAccounts;id++)
	{
		if (AccList[id].Username == User)		// Found User!
		{
			Found = true;
			break;
		}
	}



	if (Found == true)			// Update User!
	{
		m_Pass.GetWindowText(AccList[id].Password);
		AccList[id].Type = GetType(m_Type.GetCurSel());

		sprintf(queryString, "UPDATE net7_accounts SET password='%s',`account type`='%d' WHERE username='%s'", AccList[id].Password, AccList[id].Type, AccList[id].Username);
		if (!AccountLists.execute( queryString ) && AccountLists.Error())
		{
			MessageBox(AccountLists.ErrorMsg(), "Net-7 MySQL Error!", MB_ICONERROR);
		}		
		
	} else {					// New Entry!
		CString Pass, User;
		sql_query NewAccount;
		int Type = GetType(m_Type.GetCurSel());

		m_Pass.GetWindowText(Pass);
		m_User.GetWindowText(User);

		NewAccount.SetTable("net7_accounts");
		NewAccount.AddData("username", (LPCTSTR) User);
		NewAccount.AddData("password", (LPCTSTR) Pass);
		NewAccount.AddData("account type", Type);

		sprintf(queryString, "%s", NewAccount.CreateQuery());
		if (!AccountLists.execute( queryString ) && AccountLists.Error())
		{
			MessageBox(AccountLists.ErrorMsg(), "Net-7 MySQL Error!", MB_ICONERROR);
		}
		LoadUsers();
	}
	
}


int Tab2::GetType(int Index)
{
	int Type[10] = { 100, 80, 70, 60, 50, 30, 0, 10, 20, -1 };

	if (Index > 9)
		Index = 9;

	return Type[Index];
}

int Tab2::GetIndex(int Type)
{
	int Index = 0;
	int Types[10] = { 100, 80, 70, 60, 50, 30, 0, 10, 20, -1 };

	for(Index=0;Index<9;Index++)
	{
		if (Types[Index] == Type)
		{
			break;
		}
	}

	return Index;
}

void Tab2::LoadUsers()
{
	if (!m_Connected)
	{
		return;
	}

	char queryString[1024];
	sql_query_c AccountLists ( &m_MySQL_Connect );
	sql_result_c *Account_result = 0;

	sprintf(queryString, "SELECT * FROM net7_accounts");
	if ( !AccountLists.execute( queryString ) )
	{
		MessageBox("Error running MySQL Query!", "Net-7 MySQL", MB_ICONERROR);
		return;
	}

	m_User.ResetContent();


	Account_result = AccountLists.store();
	
	if (!Account_result->n_rows())
		return;

	sql_row_c AccountData;
	int id = 0;

	for(int x=0;x<Account_result->n_rows();x++)
	{
		AccountData = Account_result->fetch_row();	// Read in first row

		AccountData.allow_null(0);

		AccList[id].LastLogin = (char *) AccountData["lastlogin"];
		AccList[id].Username = (char *) AccountData["username"];
		AccList[id].Password = (char *) AccountData["password"];
		AccList[id].Type = AccountData["account type"];
		AccList[id].AccountID = (int) AccountData["userid"];

		m_User.AddString(AccountData["username"]);
		id++;
	}
	m_MaxAccounts = id;

	AccountLists.free_result();	// Free Data
}

void Tab2::OnSelchangeUser() 
{
	int UserID = m_User.GetCurSel();

	if (UserID > m_MaxAccounts) {
		UserID = m_MaxAccounts;
	}
	m_Pass.SetWindowText(AccList[UserID].Password);

	int Index = GetIndex(AccList[UserID].Type);

	m_LastLogin.SetWindowText(AccList[UserID].LastLogin);

	m_Type.SetCurSel(Index);
}

void Tab2::OnUpdateUsers() 
{
	if (!m_Connected)
	{
		MessageBox("Must connect to MySQL!", "Net-7 MySQL", MB_ICONERROR);
		return;
	}

	LoadUsers();
}

void Tab2::OnDelete() 
{
	if (!m_Connected)
	{
		return;
	}

	char queryString[1024];
	sql_query_c AccountLists ( &m_MySQL_Connect );
	sql_result_c *Account_result = 0;

	// -- See if user is in the list allready
	bool Found = false;
	CString User;

	m_User.GetWindowText(User);

	for(int id=0;id<m_MaxAccounts;id++)
	{
		if (AccList[id].Username == User)		// Found User!
		{
			Found = true;
			break;
		}
	}

	if (Found)
	{
		sprintf(queryString, "Are you sure you want to delete %s?", User);
		if (MessageBox(queryString, "Net-7 MySQL", MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
		{
			return;
		}

		sprintf(queryString, "DELETE FROM net7_accounts WHERE username='%s'", User);
		AccountLists.execute( queryString );
		LoadUsers();
	}
}


void Tab2::AppendToServerFile(char *file, char *hostname)
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

