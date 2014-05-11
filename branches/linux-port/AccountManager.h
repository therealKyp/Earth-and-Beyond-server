// AccountManager.h

#ifndef _ACCOUNT_MANAGER_H_INCLUDED_
#define _ACCOUNT_MANAGER_H_INCLUDED_

#include "Net7.h"
#include "Mutex.h"
#include "PacketStructures.h"

#define MAX_ACCOUNTS	1024
#define TICKET_EXPIRE_TIME  300000  //5 minutes (in milliseconds)

#define G_ERROR_BANNED_ACCOUNT		0
#define G_ERROR_NICKNAME_USED		1
#define	G_ERROR_INVALID_CHARS		2
#define	G_ERROR_TOO_SHORT			3
#define	G_ERROR_ONE_VOWEL			4
#define	G_ERROR_REPEATING_CHAR		5
#define	G_ERROR_RESTRICTED_LIST		6
#define	G_ERROR_TICKET_INVALID		7
#define	G_ERROR_AUTH_SERVER_DOWN	8
#define G_ERROR_INACTIVE_ACCOUNT	9
#define	G_ERROR_RESTRICTED_SHIP		10
#define	G_ERROR_NET7_INTERNAL		11


//Returns the avatar id from a given account and a slot (0-4)
#define AVATAR_ID(account_id, slot) (account_id * 5 + slot + 1)

//Returns the account id from a given avatar id
#define ACCOUNT_ID(avatar_id) (avatar_id - 1) / 5

class AccountManager
{
public:
	AccountManager();
	~AccountManager();

public:
	char  * IssueTicket(char *username, char *password);
    char  *	GetUsernameFromTicket(char *ticket);

	long	GetAccountID(char *username);
	long	GetAvatarID(char *username, int slot);

	bool	SetAccountStatus(char *username, long status);
    long	GetAccountStatus(char *username);
	bool	ChangePassword(char *username, char *password);
	
	bool	AddUser(char *username, char *password, char *access);

    long    CreateCharacter(GlobalCreateCharacter * create);
    void    DeleteCharacter(long avatar_id);

    bool    SaveDatabase(CharacterDatabase * database, long avatar_id);
    bool    ReadDatabase(CharacterDatabase * database, long avatar_id);

    void    BuildAvatarList(GlobalAvatarList * list, long account_id);

private:
    struct AccountTicket
    {
        AccountTicket * next;
        char *username;
        char ticket[64];
        unsigned long expire_time;
    } ATTRIB_PACKED;

	void	LoadAccounts();
    void    SetupTickets();

    char  * BuildTicket(char *username);
	long    ValidateAccount(char *username, char *password);

    bool    IsUsernameUnique(char *name);

	bool	UpdateTicket(int Index, char * Ticket);
	void	UpdateTime(long account_id);

    //Methods for use in NON-SQL mode only
#ifndef USE_MYSQL_ACCOUNT_DATA

    struct _User
    {
	    char * UserName;
	    char * Password;
	    char * Ticket;
	    bool InUse;
	    long Status;	//0=Player 10=donor 20=Helper 30=Beta Tester 50=GM 60=DGM 70=HGM 80=developer 100=Admin -1=Banned -2=Disabled
	    long ID;
    } ATTRIB_PACKED;

     AccountManager::_User * GetUserFromID(long account_id);
     AccountManager::_User * GetUserFromUsername(char *username);

    _User m_Accounts[MAX_ACCOUNTS];
	int m_NumAccounts;

public:
    int NumAccounts();
    long GetAccountID(int index);

    bool CreateCharacterDatabasePath(char *buffer, long avatar_id);

#endif

private:
    AccountTicket * m_Tickets;
    Mutex m_Mutex;
};

#endif // _ACCOUNT_MANAGER_H_INCLUDED_
