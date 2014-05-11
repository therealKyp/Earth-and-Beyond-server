// Tab3.cpp : implementation file
//

#include "stdafx.h"
#include "Net7Mysql.h"
#include "Tab3.h"
#include "Net7MysqlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Tab3 dialog


Tab3::Tab3(CWnd* pParent /*=NULL*/)
	: CDialog(Tab3::IDD, pParent)
{
	//{{AFX_DATA_INIT(Tab3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Tab3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Tab3)
	DDX_Control(pDX, IDC_DAT_FILE, m_DatFile);
	DDX_Control(pDX, IDC_NPC, m_NpcList);
	DDX_Control(pDX, IDC_TERMS, m_Terms);
	DDX_Control(pDX, IDC_ROOMS, m_Rooms);
	DDX_Control(pDX, IDC_NPCS, m_Npcs);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Tab3, CDialog)
	//{{AFX_MSG_MAP(Tab3)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_ADD_MYSQL, OnAddMysql)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Tab3 message handlers

void Tab3::OnBrowse() 
{
	CFileDialog open_dlg(TRUE,NULL,"LoungeNPC_*.dat",OFN_OVERWRITEPROMPT,"LoungeNPC Files (LoungeNPC_*.dat)|LoungeNPC_*.dat||");
    int iRet = open_dlg.DoModal();
    CString file_name;
    file_name = open_dlg.GetPathName();

    if(iRet == IDOK)
	{
		m_DatFile.SetWindowText(file_name);
	}	
	
}

void Tab3::OnAddMysql() 
{
	char file_name[256];
	unsigned char Data[15];
	std::string NPCList;
	bool data_read = false;

	struct StationLounge LoungeData;			// Stores the Lounge Information from the file

	memset(&LoungeData, 0, sizeof(LoungeData));	// Zero data out
	//memset(NPCList, 0, sizeof(NPCList));		// Zero out NPCList

	m_DatFile.GetWindowText(file_name, sizeof(file_name));

	if (!m_Connected)
	{
		MessageBox("Must connect to MySQL!", "Net-7 MySQL", MB_ICONERROR);
		return;
	}

	if (file_name[0] == 0)
	{
		MessageBox("Must specify a filename!", "Net-7 MySQL", MB_ICONERROR);
		return;
	}


    // Read the database from the character file
    FILE *f = fopen(file_name, "rb");
    if (f)
    {
		fread(Data, 4, 1, f);		// Cut out the opcode and size
		fread(&LoungeData.Station, sizeof(LoungeData.Station), 1, f);		// Read in room number/StationType

		for(int j=0;j<LoungeData.Station.RoomNumber;j++)					// Read in all the rooms
		{
			fread(&LoungeData.Rooms[j], sizeof(LoungeData.Rooms[j]), 1, f);
		}

		fread(&LoungeData.NumTerms, sizeof(LoungeData.NumTerms), 1, f);		// Read in the number or Terminals

		for(j=0;j<LoungeData.NumTerms;j++)									// Read in all the Terminals
		{
			fread(&LoungeData.Terms[j], sizeof(LoungeData.Terms[j]), 1, f);
		}

		fread(&LoungeData.NumNPCs, sizeof(LoungeData.NumNPCs), 1, f);		// Read in the NPC's

		for(j=0;j<LoungeData.NumNPCs;j++)									// Read in all the NPC's
		{
			fread(&LoungeData.NPC[j], 1, sizeof(LoungeData.NPC[j]), f);
			NPCList.append(LoungeData.NPC[j].Avatar.avatar_first_name);
			NPCList.append("\r\n");
		}

		m_NpcList.SetWindowText(NPCList.c_str());

		// Write out simple information for user to see
		char CharData[20];

		sprintf(CharData, "%d", LoungeData.Station.RoomNumber);
		m_Rooms.SetWindowText(CharData);
		sprintf(CharData, "%d", LoungeData.NumTerms);
		m_Terms.SetWindowText(CharData);
		sprintf(CharData, "%d", LoungeData.NumNPCs);
		m_Npcs.SetWindowText(CharData);

		data_read = true;

        fclose(f);
    }


	if (data_read)
	{
		char queryString[5000];
		sql_query_c StarBase ( &m_MySQL_Connect );



		// Read in SectorID
		file_name[strlen(file_name)-4] = 0;
		int StarBaseSector = atoi(&file_name[strlen(file_name)-5]);

		int StarbaseID = 0;

		// Write StarBases Table
		sql_query StarBases;

		StarBases.SetTable("starbases");
		StarBases.AddData("type", LoungeData.Station.StationType);
		StarBases.AddData("Sector_id", (int)StarBaseSector/10);
		StarBases.AddData("is_active", "1");
		StarBases.AddData("faction_id", "1");
		StarBases.AddData("target_sector_object", "10");
		
		sprintf(queryString, "%s", StarBases.CreateQuery() );

		if ( !StarBase.execute( queryString ) && StarBase.Error())
		{
			MessageBox(StarBase.ErrorMsg(), "Error running MySQL Query! (StarBases)", MB_ICONERROR);
			m_NpcList.SetWindowText(queryString);
			return;
		}

		// Get StarBaseID
		StarbaseID = (int)StarBase.insert_id();

		// Write rooms table
		int RoomID = 0;

		for(int roomn=0;roomn<LoungeData.Station.RoomNumber;roomn++)
		{
			int NPCIndex = 0;
			int TerminalIndex = 0;

			sql_query StarBases_Rooms;

			StarBases_Rooms.SetTable("starbase_rooms");

			StarBases_Rooms.AddData("type", LoungeData.Rooms[roomn].RoomNumber);
			StarBases_Rooms.AddData("style", LoungeData.Rooms[roomn].RoomStyle);
			StarBases_Rooms.AddData("fog_near", LoungeData.Rooms[roomn].FogNear);
			StarBases_Rooms.AddData("fog_far", LoungeData.Rooms[roomn].FogFar);
			StarBases_Rooms.AddData("starbase_id", StarbaseID);
					
			sprintf(queryString, "%s", StarBases_Rooms.CreateQuery());
			
			if ( !StarBase.execute( queryString ) && StarBase.Error())
			{
				MessageBox("Error running MySQL Query! (StarBases Rooms)", "Net-7 MySQL", MB_ICONERROR);
				m_NpcList.SetWindowText(queryString);
				return;
			}

			// Get RoomID from Auto Incroment
			RoomID = (int)StarBase.insert_id();

			// Write terminals table
			for(int termn=0;termn<LoungeData.NumTerms;termn++)
			{
				// See if its our room
				if (LoungeData.Terms[termn].RoomNumber == roomn)
				{
					sql_query StarBases_Terms;

					StarBases_Terms.SetTable("starbase_terminals");

					StarBases_Terms.AddData("location", LoungeData.Terms[termn].Location);
					StarBases_Terms.AddData("type", LoungeData.Terms[termn].TermType);
					StarBases_Terms.AddData("attribute", LoungeData.Terms[termn].Unknown);
					StarBases_Terms.AddData("room_id", RoomID);
					StarBases_Terms.AddData("terminal_index", TerminalIndex);

							
					sprintf(queryString, "%s", StarBases_Terms.CreateQuery() );
					
					if ( !StarBase.execute( queryString ) && StarBase.Error())
					{
						MessageBox(queryString, "Error running MySQL Query! (StarBases Terms)", MB_ICONERROR);
						m_NpcList.SetWindowText(queryString);
						return;
					}
					TerminalIndex++;
				}
			}

			// Write NPC table
			for(int npcn=0;npcn<LoungeData.NumNPCs;npcn++)
			{
				// See if its our room
				if (LoungeData.NPC[npcn].RoomNumber == roomn)
				{

					int NPCid = 0;

					// Write AvatarID
					sql_query AvatarInfo;

					AvatarInfo.SetTable("npc_avatar_templates");

					//AvatarInfo.AddData("avatar_template_id", NPCid);
					AvatarInfo.AddData("avatar_type",LoungeData.NPC[npcn].Avatar.avatar_type);
					AvatarInfo.AddData("avatar_version",(int)LoungeData.NPC[npcn].Avatar.avatar_version);
					AvatarInfo.AddData("race",LoungeData.NPC[npcn].Avatar.race);
					AvatarInfo.AddData("profession",LoungeData.NPC[npcn].Avatar.profession);
					AvatarInfo.AddData("gender",LoungeData.NPC[npcn].Avatar.gender);
					AvatarInfo.AddData("mood_type",LoungeData.NPC[npcn].Avatar.mood_type);
					AvatarInfo.AddData("personality",(int)LoungeData.NPC[npcn].Avatar.personality);
					AvatarInfo.AddData("nlp",(int)LoungeData.NPC[npcn].Avatar.nlp);
					AvatarInfo.AddData("shirt_type",(int)LoungeData.NPC[npcn].Avatar.body_type);
					AvatarInfo.AddData("pants_type",(int)LoungeData.NPC[npcn].Avatar.pants_type);
					AvatarInfo.AddData("head_type",(int)LoungeData.NPC[npcn].Avatar.head_type);
					AvatarInfo.AddData("hair_type",(int)LoungeData.NPC[npcn].Avatar.hair_num);
					AvatarInfo.AddData("ear_type",(int)LoungeData.NPC[npcn].Avatar.ear_num);
					AvatarInfo.AddData("goggle_num",(int)LoungeData.NPC[npcn].Avatar.goggle_num);
					AvatarInfo.AddData("beard_num",(int)LoungeData.NPC[npcn].Avatar.beard_num);
					AvatarInfo.AddData("weapon_hip_num",(int)LoungeData.NPC[npcn].Avatar.weapon_hip_num);
					AvatarInfo.AddData("weapon_unique_num",(int)LoungeData.NPC[npcn].Avatar.weapon_unique_num);
					AvatarInfo.AddData("weapon_back_num",(int)LoungeData.NPC[npcn].Avatar.weapon_back_num);
					AvatarInfo.AddData("head_texture_num",(int)LoungeData.NPC[npcn].Avatar.head_texture_num);
					AvatarInfo.AddData("tatoo_texture_num",(int)LoungeData.NPC[npcn].Avatar.tattoo_texture_num);
					AvatarInfo.AddData("tatoo_x",LoungeData.NPC[npcn].Avatar.tattoo_offset[0]);
					AvatarInfo.AddData("tatoo_y",LoungeData.NPC[npcn].Avatar.tattoo_offset[1]);
					AvatarInfo.AddData("tatoo_z",LoungeData.NPC[npcn].Avatar.tattoo_offset[2]);
					AvatarInfo.AddData("hair_color_0",LoungeData.NPC[npcn].Avatar.hair_color[0]);
					AvatarInfo.AddData("hair_color_1",LoungeData.NPC[npcn].Avatar.hair_color[1]);
					AvatarInfo.AddData("hair_color_2",LoungeData.NPC[npcn].Avatar.hair_color[2]);
					AvatarInfo.AddData("beard_color_0",LoungeData.NPC[npcn].Avatar.beard_color[0]);
					AvatarInfo.AddData("beard_color_1",LoungeData.NPC[npcn].Avatar.beard_color[1]);
					AvatarInfo.AddData("beard_color_2",LoungeData.NPC[npcn].Avatar.beard_color[2]);
					AvatarInfo.AddData("eye_color_0",LoungeData.NPC[npcn].Avatar.eye_color[0]);
					AvatarInfo.AddData("eye_color_1",LoungeData.NPC[npcn].Avatar.eye_color[1]);
					AvatarInfo.AddData("eye_color_2",LoungeData.NPC[npcn].Avatar.eye_color[2]);
					AvatarInfo.AddData("skin_color_0",LoungeData.NPC[npcn].Avatar.skin_color[0]);
					AvatarInfo.AddData("skin_color_1",LoungeData.NPC[npcn].Avatar.skin_color[1]);
					AvatarInfo.AddData("skin_color_2",LoungeData.NPC[npcn].Avatar.skin_color[2]);
					AvatarInfo.AddData("shirt1_color_0",LoungeData.NPC[npcn].Avatar.shirt_primary_color[0]);
					AvatarInfo.AddData("shirt1_color_1",LoungeData.NPC[npcn].Avatar.shirt_primary_color[1]);
					AvatarInfo.AddData("shirt1_color_2",LoungeData.NPC[npcn].Avatar.shirt_primary_color[2]);
					AvatarInfo.AddData("shirt2_color_0",LoungeData.NPC[npcn].Avatar.shirt_secondary_color[0]);
					AvatarInfo.AddData("shirt2_color_1",LoungeData.NPC[npcn].Avatar.shirt_secondary_color[1]);
					AvatarInfo.AddData("shirt2_color_2",LoungeData.NPC[npcn].Avatar.shirt_secondary_color[2]);
					AvatarInfo.AddData("pants1_color_0",LoungeData.NPC[npcn].Avatar.pants_primary_color[0]);
					AvatarInfo.AddData("pants1_color_1",LoungeData.NPC[npcn].Avatar.pants_primary_color[1]);
					AvatarInfo.AddData("pants1_color_2",LoungeData.NPC[npcn].Avatar.pants_primary_color[2]);
					AvatarInfo.AddData("pants2_color_0",LoungeData.NPC[npcn].Avatar.pants_secondary_color[0]);
					AvatarInfo.AddData("pants2_color_1",LoungeData.NPC[npcn].Avatar.pants_secondary_color[1]);
					AvatarInfo.AddData("pants2_color_2",LoungeData.NPC[npcn].Avatar.pants_secondary_color[2]);
					AvatarInfo.AddData("shirt1_metal",LoungeData.NPC[npcn].Avatar.shirt_primary_metal);
					AvatarInfo.AddData("shirt2_metal",LoungeData.NPC[npcn].Avatar.shirt_secondary_metal);
					AvatarInfo.AddData("pants1_metal",LoungeData.NPC[npcn].Avatar.pants_primary_metal);
					AvatarInfo.AddData("pants2_metal",LoungeData.NPC[npcn].Avatar.pants_secondary_metal);
					AvatarInfo.AddData("body_weight0",LoungeData.NPC[npcn].Avatar.BodyWeight[0]);
					AvatarInfo.AddData("body_weight1",LoungeData.NPC[npcn].Avatar.BodyWeight[1]);
					AvatarInfo.AddData("body_weight2",LoungeData.NPC[npcn].Avatar.BodyWeight[2]);
					AvatarInfo.AddData("body_weight3",LoungeData.NPC[npcn].Avatar.BodyWeight[3]);
					AvatarInfo.AddData("body_weight4",LoungeData.NPC[npcn].Avatar.BodyWeight[4]);
					AvatarInfo.AddData("head_weight0",LoungeData.NPC[npcn].Avatar.HeadWeight[0]);
					AvatarInfo.AddData("head_weight1",LoungeData.NPC[npcn].Avatar.HeadWeight[1]);
					AvatarInfo.AddData("head_weight2",LoungeData.NPC[npcn].Avatar.HeadWeight[2]);
					AvatarInfo.AddData("head_weight3",LoungeData.NPC[npcn].Avatar.HeadWeight[3]);
					AvatarInfo.AddData("head_weight4",LoungeData.NPC[npcn].Avatar.HeadWeight[4]);


					sprintf(queryString, "%s", AvatarInfo.CreateQuery() );
					if (!StarBase.execute( queryString ) && StarBase.Error())
					{
						MessageBox(StarBase.ErrorMsg(), "Error running MySQL Query! (StarBases AvatarInfo)", MB_ICONERROR);
						m_NpcList.SetWindowText(queryString);
						return;
					}
					

					NPCid = (int)StarBase.insert_id();

					sql_query StarBases_Venders;

					StarBases_Venders.SetTable("starbase_vendors");

					StarBases_Venders.AddData("vendor_id", NPCid);
					StarBases_Venders.AddData("booth_type", LoungeData.NPC[npcn].BoothType);					
							
					sprintf(queryString, "%s", StarBases_Venders.CreateQuery() );
					
					if ( !StarBase.execute( queryString ) && StarBase.Error())
					{
						MessageBox(StarBase.ErrorMsg(), "Error running MySQL Query! (StarBases Venders)", MB_ICONERROR);
						m_NpcList.SetWindowText(queryString);
						return;
					}

					sql_query StarBases_Npcs;
					
					StarBases_Npcs.SetTable("starbase_npcs");

					char FName[30], LName[30];
									
					mysql_escape_string(FName, LoungeData.NPC[npcn].Avatar.avatar_first_name, strlen(LoungeData.NPC[npcn].Avatar.avatar_first_name));
					mysql_escape_string(LName, LoungeData.NPC[npcn].Avatar.avatar_last_name, strlen(LoungeData.NPC[npcn].Avatar.avatar_last_name));

					StarBases_Npcs.AddData("first_name", FName );
					StarBases_Npcs.AddData("last_name", LName );
					StarBases_Npcs.AddData("location", LoungeData.NPC[npcn].Location);
					StarBases_Npcs.AddData("npc_Id", NPCid);
					StarBases_Npcs.AddData("faction_id", 1);
					StarBases_Npcs.AddData("room_id", RoomID);
					StarBases_Npcs.AddData("npc_index", NPCIndex);
							
					sprintf(queryString, "%s", StarBases_Npcs.CreateQuery() );
					
					if ( !StarBase.execute( queryString ) && StarBase.Error())
					{
						MessageBox(StarBase.ErrorMsg(), "Error running MySQL Query! (StarBases NPCs)", MB_ICONERROR);
						m_NpcList.SetWindowText(queryString);
						return;
					}
					NPCIndex++;
				}
			}
			
		}
		// -----------------------------------------------------



	}


	
}
