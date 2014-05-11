#if !defined(AFX_TAB3_H__78C66D2A_6FED_4A10_8F3B_B0A840CE936F__INCLUDED_)
#define AFX_TAB3_H__78C66D2A_6FED_4A10_8F3B_B0A840CE936F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tab3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Tab3 dialog

// LoungeNPC Structure
// avatar_desc.cpp L004BFED0

#pragma pack(1)
#define ATTRIB_PACKED

struct AvatarData
{
    char    avatar_first_name[20];      // 14   d4  20
    char    avatar_last_name[20];       // 28   e8  20
    int		avatar_type;                // 04   08  4
    char    filler1;                    //      0c  -
    char    avatar_version;             // 09   0d  1
                                        //      0e
                                        //      0f
    int		race;                       // 0c   10  4
    int		profession;                 // 10   14  4
    int		gender;                     // 14   18  4
    int		mood_type;                  // 18   1c  4

    char    personality;                // 1c   20  1
    char    nlp;                        // 1d   21  1
    char    body_type;                  // 1e   22  1 (shirt type?)
    char    pants_type;                 // 1f   23  1
    char    head_type;                  // 20   24  1
    char    hair_num;                   // 21   27  1
    char    ear_num;                    // 22   26  1
    char    goggle_num;                 // 23   27  1
    char    beard_num;                  // 24   28  1
    char    weapon_hip_num;             // 25   29  1
    char    weapon_unique_num;          // 26   2a  1
    char    weapon_back_num;            // 27   2b  1
    char    head_texture_num;           // 28   2c  1
    char    tattoo_texture_num;         // 29   2d  1
                                        //      2e  -
                                        //      2f  -

    float   tattoo_offset[3];           // 2c   30  12 (x,y,zoom)
    float   hair_color[3];              // 38   3c  12
    float   beard_color[3];             // 44   48  12
    float   eye_color[3];               // 50   54  12
    float   skin_color[3];              // 5c   60  12
    float   shirt_primary_color[3];     // 68   6c  12
    float   shirt_secondary_color[3];   // 74   78  12
    float   pants_primary_color[3];     // 80   84  12
    float   pants_secondary_color[3];   // 8c   90  12

    int		shirt_primary_metal;        // 98   9c  4
    int		shirt_secondary_metal;      // 9c   a0  4
    int		pants_primary_metal;        // a0   a4  4
    int		pants_secondary_metal;      // a4   a8  4

    char    filler2;                    //          1?

    float   BodyWeight[5];              //      ac  20
    float   HeadWeight[5];              //      c0  20
} ATTRIB_PACKED;  // 241 bytes

struct StationData {
	int	StationType;
	int	RoomNumber;
};

struct StationRooms {
	int RoomNumber;
	int	RoomStyle;
	float FogNear;
	float FogFar;
	int Unknown1;
	int Unknown2;
	int Unknown3;
};

// Term Number

struct StationTerms {
	int	RoomNumber;
	int Location;
	int TermType;
	int Unknown;
};

// NPC Number

struct StationNPC {
	int RoomNumber;
	int	Location;
	int NPCID;
	int BoothType;
	int Unknown1;
	int Unknown2;
	struct AvatarData Avatar;
};

struct StationLounge {
	struct StationData	Station;
	struct StationRooms	Rooms[5];
	int					NumTerms;
	struct StationTerms	Terms[7];
	int					NumNPCs;
	StationNPC	NPC[30];
};

class Tab3 : public CDialog
{
// Construction
public:
	Tab3(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Tab3)
	enum { IDD = IDD_TAB3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CEdit		m_DatFile;

	CEdit		m_Rooms;
	CEdit		m_Npcs;
	CEdit		m_Terms;
	CEdit		m_NpcList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Tab3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Tab3)
	afx_msg void OnBrowse();
	afx_msg void OnAddMysql();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB3_H__78C66D2A_6FED_4A10_8F3B_B0A840CE936F__INCLUDED_)
