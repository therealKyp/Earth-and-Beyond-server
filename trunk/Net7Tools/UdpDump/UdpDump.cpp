// UdpDump.cpp
//
// Generate SectorContent.xml including the following opcodes:
//
// opcode 0x04 Create
// opcode 0x30 ActivateRenderState <- used for activating gates
// opcode 0x09 ObjectEffect <- can have more than one object effect
// opcode 0x89 Relationship
// opcode 0x08 SimplePositionalUpdate
// opcode 0x3E AdvancedPositionalUpdate
// opcode 0x3F PlanetPositionalUpdate
// opcode 0x40 ConstantPositionalUpdate
// opcode 0x46 ComponentPositionalUpdate
// opcode 0x1B AuxDataPackets
// opcode 0x99 NavigationPacket


#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include "WestwoodRSA.h"
#include "WestwoodRC4.h"
#include "../Net7/Opcodes.h"
#include "../Net7/PacketStructures.h"

#pragma comment(lib, "wsock32")
#pragma comment(lib, "cryptlib.lib")

void DoFile(char *input, char *output);
void DumpHex(unsigned char *data, long data_length, FILE * fout);
void ProcessCORD(unsigned short session_id, unsigned char *data, long data_length, FILE *fout, bool client_to_server);
void ProcessCordMgrData(unsigned char *data, long data_length, FILE *fout, bool client_to_server);
bool ProcessOpcode(unsigned char *data, long data_length, FILE *fout, bool client_to_server);
void ProcessFullOpcodeData(unsigned char *data, long data_length);
void DumpItemBase(unsigned char *data, long data_length);
WORD CRC16(BYTE *msg, int numBytes);
void Cleanup();
void CreateSectorContentXmlFiles();

static unsigned char buffer[16384];
static WestwoodRSA westwood_rsa;
static long bytes_remaining = 0;
static unsigned int frame_number = 0;
static bool hdiv = false;
static long g_ItemTemplateID[8192] = {NULL};
static int g_NumItems = 0;
static int g_NumItemBasePackets = 0;
static unsigned char g_OpcodeData[32768];
static int g_OpcodeDataIndex = 0;
static int g_OpcodeDataLength = 0;
static int g_SectorID = 0;
static int g_NumSectors = 0;
static long g_SectorList[128];

struct SectorContent
{
    long    SectorID;           // which sector is this object in?
    long    GameID;             // unique id, ignore duplicates
    // These are in the order in which the packets should be sent
    BYTE  * CreatePacket;                   // opcode 0x04
    BYTE  * ActivateRenderStatePackets[2];   // opcode 0x30 <- used for activating gates
    BYTE  * ObjectEffectPackets[3];         // opcode 0x09 <- can have more than one object effect
    BYTE  * RelationshipPacket;             // opcode 0x89
    BYTE  * SimplePositionalUpdate;         // opcode 0x08
    BYTE  * AdvancedPositionalUpdate;       // opcode 0x3E
    BYTE  * PlanetPositionalUpdate;         // opcode 0x3F
    BYTE  * ConstantPositionalUpdate;       // opcode 0x40
    BYTE  * ComponentPositionalUpdate;      // opcode 0x46 <- not sure if this is used for space object
    BYTE  * AuxDataPackets[3];              // opcode 0x1B <- ignore all except the "Object Name" type
    BYTE  * NavigationPacket;               // opcode 0x99
    char    Removed;                        // opcode 0x07 set if this object was "Removed"
    char    IgnoreDuplicate;                // true if more than one Create seen
};

SectorContent g_SectorContentArray[8192];
SectorContent * g_SectorContent = NULL;
int g_SectorContentCount = 0;
int g_TotalCreateOpcodes = 0;

// sloppy code used to capture binary ItemBase and AuxData to a file
// for playback in docking at a station.
//
// capture3:
//   ItemBase = 238 to 351
//   AuxData = 352 to 371
//   AvatarDescription + Colorization = 372
//   Decal + NameDecal + Relationship + Advanced_Positional_Update + Aux_Data = 373 to 384
//   Io = 238 to 384
//   Friendship7 = 14070 to 14225
//   Room change = 16209 to 16214

// Capture2:
//   ItemBaseEarthStation = 234 to 370
//   ManufacturingLab = 371 to 372
//   EarthStationAuxData = 375 to 378
//   LoungeNPC = 379 to 389
//
//   ServerParameters = 803
//
// Capture1:
//   ItemBaseEarthSector = 6808 to 6922
//   EarthSectorAuxData = 6923 to 6942
//   EarthSector = 6942 to 6955
//   EarthSectorAuxData2 = 6957 to 6961
//   EarthSectorObjects = 6961 to 6998

FILE *aux_data_file = NULL;
FILE *item_base_file = NULL;
void CaptureAuxData(unsigned char *data, long data_length)
{
	if ((aux_data_file) &&
		(frame_number >= 6957) &&
		(frame_number <= 6961))
	{
		fwrite(data, 1, data_length, aux_data_file);
	}
}

#define NUM_OPCODES  238
struct opcodes
{
	long c2s_count;
	long s2c_count;
	long min_size;
	long max_size;
	_int64 accumulated_size;
	long count;
} g_opcode[NUM_OPCODES];

struct RC4Key
{
    unsigned char   session_key[8];      // obtained from the RSA key exchange
    long            udp_packet_length;   // excludes the first five bytes
    long            udp_sequence_number; // in host order (little endian)
};

struct SessionInfo;
struct SessionInfo
{
	unsigned short session_id;
	struct SessionInfo *next;
	RC4Key rc4_key;
};

static struct SessionInfo * g_SessionList = NULL;

struct CordHeader
{
    char            packet_type;      // 0x05 = CORD
    short           session_id;
    short           sequence_number;  // in network byte order (big endian)
};

struct file_header
{
	long	magic_number;
	short	major_version;
	short	minor_version;
	long	time_zone_offset;
	long	time_stamp_accuracy;
	long	snapshot_length;
	long	link_layer_type;
};

struct frame_header
{
	long	capture_time;			// seconds since Jan 1, 1970
	long	capture_ms;				// milliseconds
	unsigned int	length;			// number of bytes that follow
	unsigned int	actual_length;	// number of bytes actually captured (may be greater than length)
};

static char *opcode_string[] =
{
	{ "VersionRequest" },
	{ "VersionResponse" },
	{ "Login" },
	{ "Logoff" },
	{ "Create" },
	{ "Start" },
	{ "StartAck" },
	{ "Remove" },
	{ "Simple_Positional_Update" },
	{ "Object_Effect" },
	{ "Point_Effect" },
	{ "Object_To_Object_Effect" },
	{ "Object_To_Effect_Effect" },
	{ "Object_To_Point_Effect" },
	{ "Object_To_Object_Duration_Linked_Effect" },
	{ "Remove_Effect" },
	{ "Decal" },
	{ "Colorization" },
	{ "Turn" },
	{ "Tilt" },
	{ "Move" },
	{ "Fire" },
	{ "Fire Equipped" },
	{ "Request_Target" },
	{ "Request_Target_Target" },
	{ "Set_Target" },
	{ "Debug" },
	{ "Aux_Data" },
	{ "PlayerVar_Aux_Data" },
	{ "Message_String" },
	{ "Group_Action" },
	{ "Trade_Action" },
	{ "PriorityMessageLine" },
	{ "QueueMessageLine" },
	{ "PushMessageLine" },
	{ "SuperLine" },
	{ "ClearMessageLine" },
	{ "ItemBase" },
	{ "Change_Base_Asset" },
	{ "Inventory" },
	{ "InventorySort" },
	{ "ItemStates" },
	{ "Set_ZBand" },
	{ "Set_BBox" },
	{ "Action" },
	{ "Action2" },
	{ "Option" },
	{ "Initialize_Render_State" },
	{ "Activate_Render_State" },
	{ "Activate_Next_Render_State_Once" },
	{ "Deactivate_Render_State" },
	{ "Client_Chat" },
	{ "Set_Client_Time" },
	{ "Master_Join" },
	{ "Client_Redirect" },
	{ "Client_Avatar" },
	{ "Spline" },
	{ "NO" },
	{ "Server_Handoff" },
	{ "SectorServer_Ready" },
	{ "Set_Client_Type" },
	{ "Server_Stop" },
	{ "Advanced_Positional_Update" },
	{ "Planet_Positional_Update" },
	{ "Constant_Positional_Update" },
	{ "FormationPositionalUpdate" },
	{ "Server_Parameters" },
	{ "Request_Transform_Change" },
	{ "Request_Time" },
	{ "Request_Version" },
	{ "Component_Positional_Update" },
	{ "Client_Ship" },
	{ "CreateCharacter" },
	{ "DeleteCharacter" },
	{ "CreateAttachment" },
	{ "ModeRequest" },
	{ "ModeRequest" },
	{ "RegistrationResult" },
	{ "Starbase_Request" },
	{ "Starbase_Set" },
	{ "Starbase_Tow" },
	{ "Request_Skill_Strings" },
	{ "LoungeNPC" },
	{ "NumPlayers?" },
	{ "TalkTree" },
	{ "Select_Talk_Tree" },
	{ "TalkTreeAction" },
	{ "SkillAction" },
	{ "SkillAbility" },
	{ "HullUpgrade" },
	{ "VerbRequest" },
	{ "VerbList" },
	{ "VerbUpdate" },
	{ "UseInventoryItem" },
	{ "Chatstream" },
	{ "Message_String" },
	{ "Message_Time" },
	{ "AvatarDescription" },
	{ "Dialog" },
	{ "Broadcast" },
	{ "ClientDamage" },
	{ "UITrigger" },
	{ "Open_Interface" },
	{ "GadgetControl" },
	{ "GadgetControl" },
	{ "ClickIndicator" },
	{ "Client_Sound" },
	{ "Client_Sound_Command" },
	{ "Note" },
	{ "GlobalConnect" },
	{ "GlobalTicketRequest" },
	{ "GlobalTicket" },
	{ "GlobalAvatarList" },
	{ "GlobalDeleteCharacter" },
	{ "GlobalCreateCharacter" },
	{ "GlobalHello" },
	{ "GlobalPlayerStatus" },
	{ "GlobalError" },
	{ "GlobalLogin" },
	{ "GlobalSetReferrer" },
	{ "GlobalSetReferrerResponse" },
	{ "ManufactureItemCategoryRequest" },
	{ "ManufactureItemCategoryRequest" },
	{ "ManufactureSetItemID" },
	{ "RefinerySetItemID" },
	{ "ManufactureSetSlotID" },
	{ "ManufactureAction" },
	{ "ManufactureSetManufactureID" },
	{ "ManufactureTechLevelFilterRequest" },
	{ "RecustomizeShipStart" },
	{ "RecustomizeShipDone" },
	{ "RecustomizeAvatarStart" },
	{ "RecustomizeAvatarDone" },
	{ "RecustomizeAvatarUpdate" },
	{ "MissionForfeitRequest" },
	{ "MissionDismissRequest" },
	{ "PetitionStuck" },
	{ "Relationship" },
	{ "Player_Object_Reaction_Update" },
	{ "AttackerUpdates" },
	{ "LootHulkPermission" },
	{ "IncapacitanceDetailsRequest" },
	{ "IncapacitanceDistressBeacon" },
	{ "Request_Rescue" },
	{ "IncapacitanceDetails" },
	{ "IncapacitanceRescueConfirmation" },
	{ "CameraControl" },
	{ "JobList" },
	{ "JobDescription" },
	{ "JobDelete" },
	{ "JobAcceptReply" },
	{ "GalaxyMap" },
	{ "GalaxyMap (request)" },
	{ "Navigation" },
	{ "NavDelete" },
	{ "Warp" },
	{ "Warp_Index" },
	{ "Starbase_Avatar_Change2" },
	{ "Starbase_Avatar_Change" },
	{ "Starbase_Room_Change" },
	{ "Starbase_Room_Update" },
	{ "TriggerEmote" },
	{ "NotifyEmote" },
	{ "ClientChatRequest" },
	{ "ClientChatList" },
	{ "ClientChatEvent" },
	{ "ClientChatError" },
	{ "ChatBasicRequest" },
	{ "ChatProxyLogin" },
	{ "ChatProxyRequest" },
	{ "ChatProxyResponse" },
	{ "ChatProxyResponseList" },
	{ "ChatProxyEvent" },
	{ "GroupServerLogin" },
	{ "GroupServerRequest" },
	{ "GroupServerResponse" },
	{ "GroupServerCreate" },
	{ "GroupServerUpdate" },
	{ "NameDecal" },
	{ "ShipNameValidate" },
	{ "SubParts" },
	{ "FindPlayerRequest" },
	{ "FindPlayerResponse" },
	{ "NoteList" },
	{ "NoteContent" },
	{ "LogoffRequest" },
	{ "LogoffConfirmation" },
	{ "LogoffConfirmation2" },
	{ "CTA_Request" },
	{ "CTA_Response" },
	{ "ConfirmedActionOffer" },
	{ "ConfirmedActionTimeout" },
	{ "ConfirmedActionResponse" },
	{ "GuildPlayerOnline" },
	{ "GuildAdjustContrib" },
	{ "GuildStatusUpdate" },
	{ "GuildBeginCreation" },
	{ "GuildLeaderAcceptClient" },
	{ "GuildLeaderAcceptSector" },
	{ "GuildRecruitConfirmGuild" },
	{ "GuildRecruitConfirmSector" },
	{ "GuildRecruitAcceptClient" },
	{ "GuildRecruitAcceptSector" },
	{ "GuildSimpleGuildSector" },
	{ "GuildSimpleSectorClient" },
	{ "GuildSimpleClientSector" },
	{ "GuildSimpleSectorGuild" },
	{ "GuildMessageGuild" },
	{ "GuildMessageSector" },
	{ "GuildPermissions" },
	{ "GuildPlayerPermissions" },
	{ "GuildRankNamesSector" },
	{ "GuildRankNamesRequestClient" },
	{ "GuildRankNamesGuild" },
	{ "GuildRankNamesRequestSector" },
	{ "GuildGMRenameConfirmGuild" },
	{ "GuildGMRenameConfirmSector" },
	{ "GuildGMRenameAcceptSector" },
	{ "GuildDeletePlayer" },
	{ "GuildPlayerInfoDeleted" },
	{ "GuildDeductFee" },
	{ "GPSRequest" },
	{ "GPSResponse" },
	{ "DirLockRequest" },
	{ "DirLockResponseGlobal" },
	{ "DirLockResponseSector" },
	{ "DirLockRelease" },
	{ "GlobalLockRequest" },
	{ "GlobalLockResponse" },
	{ "GlobalLockRelease" },
	{ "GlobalContainerRequest" },
	{ "GlobalContainerResponse" },
	{ "SpendReferralPoint" },
	{ "ReferralPointsAvailable" },
	{ "GM_Edit_Player_Inventory" },
	{ "GM_View_Mission_Log" },
	{ "GM_Destroy_Item" },
	{ "GM_Reset_Mission" },
};

int main(int argc, char* argv[])
{
	memset(g_opcode, NULL, sizeof(g_opcode));
    memset(g_SectorContentArray, NULL, sizeof(g_SectorContentArray));

	char orig_path[MAX_PATH];
	GetCurrentDirectory(sizeof(orig_path), orig_path);
	SetCurrentDirectory("C:\\Net7\\_data\\");

    //item_base_file = fopen("ItemBase.xml", "w");

	aux_data_file = fopen("EarthSectorAuxData.dat", "wb");

    DoFile("capture1_ea.bin", "_capture_1_test.txt");
    //DoFile("capture2_ea.bin", "_capture_2_test.txt");
    //DoFile("capture3_ea.bin", "_capture_3_test.txt");

	if (aux_data_file)
	{
		fclose(aux_data_file);
		aux_data_file = NULL;
	}

    if (item_base_file)
    {
        fclose(item_base_file);
        item_base_file = NULL;
    }

    //printf("Found %d Item_Base packets\n", g_NumItemBasePackets);
    //printf("Found %d unique Items\n", g_NumItems);

    //CreateSectorContentXmlFiles();

    //printf("Found %d Create Opcodes\n", g_TotalCreateOpcodes);
    //printf("Found %d unique objects\n", g_SectorContentCount);

	getchar();
	SetCurrentDirectory(orig_path);

    Cleanup();

    return 0;
}

void DoFile(char *input, char *output)
{
	unsigned int frame_count_arp = 0;
	unsigned int frame_count_icmp = 0;
	unsigned int frame_count_udp = 0;
	unsigned int frame_count_tcp = 0;
	unsigned int frame_count_dns = 0;
	unsigned int frame_count_ea = 0;
	unsigned int frame_count_netbios = 0;
	unsigned int frame_count_ntp = 0;
	unsigned int frame_count_microsoft = 0;
	unsigned int frame_count_home = 0;
	unsigned int frame_count_unknown = 0;
	unsigned int frame_count_m_search = 0;
	unsigned int frame_count_keepers = 0;
	unsigned int frame_count_discarded = 0;
	unsigned int i;
	int port = 0;
	int lastport = -1;
	int switchport = -1;
	char server_ip[32];
	bool first_time = true;
	frame_header frame;

	FILE *f = fopen(input, "rb");
	if (f)
	{
		fread(buffer, 1, sizeof(file_header), f);
		file_header *head = (file_header *) buffer;
		if (head->magic_number != 0xa1b2c3d4)	// libpcap magic number
		{
			printf("Missing magic number\n");
			return;
		}
		if (head->snapshot_length != 0xffff)	// full capture
		{
			printf("Invalid snapshot length\n");
			return;
		}
		if (head->link_layer_type != 1)			// ethernet
		{
			printf("Invalid link layer type\n");
			return;
		}
		FILE *fout = fopen(output, "w");
		if (!fout)
		{
			fclose(f);
			printf("Error opening output file\n");
			return;
		}

		while (!feof(f))
		{
			if (fread(&frame, 1, sizeof(frame), f) == sizeof(frame))
			{
				frame_number++;
				char *direction = "";
				bool client_to_server = false;
				unsigned int length = frame.length;
				if (length != frame.actual_length)
				{
					printf("Frame #%d : frame length != actual length\n", frame_number);
					return;
				}
				if (fread(buffer, 1, length, f) == length)
				{
					bool recognized = false;
					bool keeper = false;
					if ((buffer[12] == 8) && (buffer[13] == 6))
					{
						frame_count_arp++;
						recognized = true;
					}
					if ((buffer[12] == 8) && (buffer[13] == 0))
					{
						// this is an IP packet
						if (buffer[23] == 1)
						{
							// This is an ICMP packet
							frame_count_icmp++;
							recognized = true;
						}
						if (buffer[23] == 17)
						{
							// This is a UDP packet
							frame_count_udp++;
							if ((buffer[36] == 7) && (buffer[37] == 108) &&
								(buffer[30] == 192) && (buffer[31] == 168) &&
								(buffer[32] == 254) && (buffer[33] == 254))
							{
								// M-SEARCH packet to the router
								frame_count_m_search++;
								recognized = true;
							}
							else if ((buffer[36] == 0) && (buffer[37] == 53))
							{
								// This is a DNS query
								frame_count_dns++;
								recognized = true;
							}
							else if ((buffer[34] == 0) && (buffer[35] == 53))
							{
								// This is a DNS response
								frame_count_dns++;
								recognized = true;
							}
							if ((buffer[30] == 159) && (buffer[31] == 153))
							{
								// This is UDP message to EA
								frame_count_ea++;
								recognized = true;
								direction = "Client->Server";
								client_to_server = true;
								port = buffer[36] * 256 + buffer[37];
								keeper = true;
								sprintf(server_ip, "%d.%d.%d.%d",
									buffer[30], buffer[31], buffer[32], buffer[33]);
							}
							else if ((buffer[26] == 159) && (buffer[27] == 153))
							{
								// This is UDP message from EA
								frame_count_ea++;
								recognized = true;
								direction = "Server->Client";
								port = buffer[34] * 256 + buffer[35];
								keeper = true;
								sprintf(server_ip, "%d.%d.%d.%d",
									buffer[26], buffer[27], buffer[28], buffer[29]);
							}
							else if ((buffer[36] == 0) && (buffer[37] == 137))
							{
								// This is a netbios-ns message
								frame_count_netbios++;
								recognized = true;
							}
							else if ((buffer[34] == 0) && (buffer[35] == 137))
							{
								// This is a netbios-ns message
								frame_count_netbios++;
								recognized = true;
							}
							else if ((buffer[36] == 0) && (buffer[37] == 138))
							{
								// This is a netbios-dgm message
								frame_count_netbios++;
								recognized = true;
							}
							else if ((buffer[34] == 0) && (buffer[35] == 138))
							{
								// This is a netbios-dgm message
								frame_count_netbios++;
								recognized = true;
							}
							else if ((buffer[36] == 0) && (buffer[37] == 123))
							{
								// This is a NTP message
								frame_count_ntp++;
								recognized = true;
							}
							else if ((buffer[34] == 0) && (buffer[35] == 123))
							{
								// This is a NTP message
								frame_count_ntp++;
								recognized = true;
							}
						}
						else if (buffer[23] == 6)
						{
					        unsigned int data_offset = 34 + (buffer[46] >> 2);
					        unsigned int data_length = length - data_offset;
							// This is a TCP packet
							frame_count_tcp++;
							if ((buffer[30] == 159) && (buffer[31] == 153))
							{
								// This is TCP message to EA
								frame_count_ea++;
								recognized = true;
                                /*
								direction = "Client->Server";
								client_to_server = true;
								sprintf(server_ip, "%d.%d.%d.%d",
									buffer[30], buffer[31], buffer[32], buffer[33]);
								port = buffer[34] * 256 + buffer[35];
                                if (port == 3805)
                                {
                                    unsigned char modulo_sig[8] =
                                    { 0, 0, 0, 0x41, 0, 0xC6, 0x4B, 0xA5 };
                                    if ((data_length == 74) &&
                                        (memcmp(&buffer[data_offset], modulo_sig, 8) == 0))
                                    {
                                        DumpHex(&buffer[data_offset], 74, fout);
                                    }
                                }
                                */
							}
							else if ((buffer[26] == 159) && (buffer[27] == 153))
							{
								// This is TCP message from EA
								sprintf(server_ip, "%d.%d.%d.%d",
									buffer[26], buffer[27], buffer[28], buffer[29]);
								frame_count_ea++;
								recognized = true;
							}
							else if ((buffer[30] == 207) && (buffer[31] == 46))
							{
								// This is TCP message to Microsoft
								frame_count_microsoft++;
								recognized = true;
							}
							else if ((buffer[26] == 207) && (buffer[27] == 46))
							{
								// This is TCP message from Microsoft
								frame_count_microsoft++;
								recognized = true;
							}
							else if ((buffer[36] == 0) && (buffer[37] == 139))
							{
								// This is a netbios-ssn message
								frame_count_netbios++;
								recognized = true;
							}
							else if ((buffer[34] == 0) && (buffer[35] == 139))
							{
								// This is a netbios-ssn message
								frame_count_netbios++;
								recognized = true;
							}
						}
					}
					if (!recognized)
					{
						printf("Unrecognized packet, Frame #%d\n", frame_number);
					}
					else if (keeper)
					{
						unsigned int data_offset = 42;
						unsigned int data_length = buffer[38] * 256 + buffer[39] - 8;

						if (data_length > 0)
						{
							bool newport = (port != lastport);
							if (port == switchport)
							{
								newport = false;
							}
							if (newport && (buffer[0] == 0) && !first_time)
							{
								fprintf(fout, "----------------------\n");
								fprintf(fout, "--  Server Handoff  --\n");
								fprintf(fout, "----------------------\n\n");
							}
                            fprintf(fout, "-----------------------------------------------------------\n");
							fprintf(fout, "Packet #%d: %d bytes, %s  %s:%d\n",
								frame_number, data_length, direction,
								server_ip, port);
                            fprintf(fout, "-----------------------------------------------------------\n\n");
                            if (frame_number == 1421)
                            {
                                newport = false;
                            }
							if (port != switchport)
							{
								lastport = port;
							}
							unsigned char *data = &buffer[data_offset];
							switch (data[0])
							{
							case 0 :
								if (!newport)
								{
									fprintf(fout, " Warning: Found SYN1 using the same port!\n");
								}
								newport = false;
								first_time = false;
								fprintf(fout, " %02X               SYN1\n", data[0]);
								fprintf(fout, " %02X               -unknown-\n", data[1]);
								fprintf(fout, " %02X %02X            Session ID\n", data[2], data[3]);
								if (data_length != 4)
								{
									fprintf(fout, " Warning: expected data length to be 4!\n");
								}
								break;

							case 1 :
								fprintf(fout, " %02X               ACK1\n", data[0]);
								fprintf(fout, " %02X               -unknown-\n", data[1]);
								fprintf(fout, " %02X %02X            Session ID\n", data[2], data[3]);
								fprintf(fout, " %02X %02X %02X %02X      -unknown-\n", data[4], data[5], data[6], data[7]);
								fprintf(fout, " %02X %02X %02X %02X      -unknown-\n", data[8], data[9], data[10], data[11]);
								fprintf(fout, " %02X %02X %02X %02X      Modulo Length\n", data[12], data[13], data[14], data[15]);
								fprintf(fout, " %02X               Modulo MSB\n", data[16]);
								data += 17;
								for (i = 0; i < 64; i++)
								{
									fprintf(fout, " %02X", data[i]);
									if ((i % 16) == 15)
									{
										fprintf(fout, "\n");
									}
								}
								data += 64;
								fprintf(fout, " %02X %02X %02X %02X      Exponent Length\n", data[0], data[1], data[2], data[3]);
								fprintf(fout, " %02X               Exponent\n", data[4]);
								if (data_length != 86)
								{
									fprintf(fout, " Warning: expected data length to be 86!\n");
								}
								break;

							case 2 :
                                {
									unsigned short session_id = data[2] * 256 + data[3];
								    fprintf(fout, " %02X               SYN2\n", data[0]);
								    fprintf(fout, " %02X               -unknown-\n", data[1]);
								    fprintf(fout, " %02X %02X            Session ID\n", data[2], data[3]);
								    fprintf(fout, " %02X %02X %02X %02X      -unknown-\n", data[4], data[5], data[6], data[7]);
								    fprintf(fout, " %02X %02X %02X %02X      -unknown-\n", data[8], data[9], data[10], data[11]);
								    fprintf(fout, " %02X %02X %02X %02X      -unknown-\n", data[12], data[13], data[14], data[15]);
								    fprintf(fout, " %02X %02X %02X %02X      Session Key Length\n", data[16], data[16], data[16], data[19]);
                                    if (data[19] == 65)
                                    {
    								    fprintf(fout, " %02X               session key msb\n", data[20]);
                                        data++;
                                        data_length--;
                                    }
								    data += 20;
                                    unsigned char *rc4_ptr = data;
                                    for (i = 0; i < 64; i++)
								    {
									    fprintf(fout, " %02X", data[i]);
									    if ((i % 16) == 15)
									    {
										    fprintf(fout, "\n");
									    }
								    }
								    if (data_length != 84)
								    {
									    fprintf(fout, " Warning: expected data length to be 84!\n");
								    }

					                unsigned char rc4key[WWRSA_BLOCK_SIZE];
					                if (westwood_rsa.Decrypt(rc4_ptr, WWRSA_BLOCK_SIZE, rc4key))
					                {
						                // Reverse the order of the decrypted RC4 Session Key
										SessionInfo * session = new SessionInfo;
										session->session_id = session_id;
										session->next = NULL;
										memset(&session->rc4_key, NULL, sizeof(RC4Key));
										if (g_SessionList)
										{
											// Add this session to the beginning of the linked list
											session->next = g_SessionList;
											g_SessionList = session;
										}
										else
										{
											// This is the first session in the linked list
											g_SessionList = session;
										}

                                        unsigned char *p = session->rc4_key.session_key;
						                p[0] = rc4key[0x3f];
						                p[1] = rc4key[0x3e];
						                p[2] = rc4key[0x3d];
						                p[3] = rc4key[0x3c];
						                p[4] = rc4key[0x3b];
						                p[5] = rc4key[0x3a];
						                p[6] = rc4key[0x39];
						                p[7] = rc4key[0x38];

                                        fprintf(fout, "RC4 session key:\n");
                                        fprintf(fout, " %02X %02X %02X %02X %02X %02X %02X %02X\n",
                                            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                                    }
                                }
                                break;

							case 3 :
								fprintf(fout, " %02X               ACK2\n", data[0]);
								fprintf(fout, " %02X               -unknown-\n", data[1]);
								fprintf(fout, " %02X %02X            Session ID\n", data[2], data[3]);
								fprintf(fout, " %02X %02X            -unknown-\n", data[4], data[5]);
								switchport = data[6] * 256 + data[7];
								fprintf(fout, " %02X %02X            CORD Port Number (%d)\n", data[6], data[7], switchport);
								fprintf(fout, " %02X %02X %02X %02X      -unknown-\n", data[8], data[9], data[10], data[11]);
								if (data_length != 12)
								{
									fprintf(fout, " Warning: expected data length to be 86!\n");
								}
								break;

							case 4 :
							case 5 :
								{
									unsigned short session_id = data[1] * 256 + data[2];
			                        ProcessCORD(session_id, data, data_length, fout, client_to_server);
									break;
								}

							case 6 :
								fprintf(fout, " %02X               GO_AWAY\n", data[0]);
								data++;
								data_length--;
                                DumpHex(data, data_length, fout);
								break;

							default :
								fprintf(fout, " %02X               UNKNOWN\n", data[0]);
								data++;
								data_length--;
								for (i = 0; i < data_length; i++)
								{
									fprintf(fout, " %02X", data[i]);
									if ((i % 16) == 15)
									{
										fprintf(fout, "\n");
									}
								}
								if (data_length % 16)
								{
									fprintf(fout, "\n");
								}
								break;
							}

							if (newport)
							{
								fprintf(fout, " Warning: unexpected port change!\n");
							}

							fprintf(fout, "\n");
						}
						frame_count_keepers++;
					}
					else
					{
						frame_count_discarded++;
					}
				}
				else
				{
					printf("fread failed on Frame #%d\n", frame_number);
					return;
				}
			}
			else if (!feof(f))
			{
				printf("fread failed on header for Frame #%d\n", frame_number);
				return;
			}
			//if (frame_count_keepers > 5000)
			//{
			//	break;
			//}
		}

		fprintf(fout, "------------------------------------------------\n");
		fprintf(fout, "Opcode Summary:\n");
		fprintf(fout, "------------------------------------------------\n");
		for (int i = 0; i < NUM_OPCODES; i++)
		{
			if (g_opcode[i].count > 0)
			{
				long avg_size = (long) (g_opcode[i].accumulated_size / g_opcode[i].count);
				fprintf(fout,
					"0x%02X, %s, %d, %d, %d, %d, %d, %d\n",
					i,
					opcode_string[i],
					g_opcode[i].count,
					g_opcode[i].c2s_count,
					g_opcode[i].s2c_count,
					g_opcode[i].min_size,
					g_opcode[i].max_size,
					avg_size);
			}
		}
		fclose(fout);
		fclose(f);
	}
	else
	{
		printf("Error opening capture1.UDP\n");
	}

	printf("Read %d frames\n", frame_number);
}

void DumpHex(unsigned char *data, long data_length, FILE * fout)
{
    long i, j;
	for (i = 0; i < data_length; i++)
	{
		fprintf(fout, " %02X", data[i]);
		if ((i % 16) == 15)
		{
			fprintf(fout, "   ");
            for (j = 0; j < 16; j++)
            {
                char c = (char) data[i+j-15];
                if (!isprint(c))
                {
                    c = '.';
                }
                fprintf(fout, "%c", c);
            }
            fprintf(fout, "\n");
		}
	}
    long x = data_length % 16;
	if (x)
	{
        long n = (16 - (x)) * 3;
        fprintf(fout, "%*.*s", n, n, "                                                    ");
		fprintf(fout, "   ");
        for (j = 0; j < x; j++)
        {
            char c = (char) data[i+j-x];
            if (!isprint(c))
            {
                c = '.';
            }
            fprintf(fout, "%c", c);
        }
		fprintf(fout, "\n");
	}
    hdiv = true;
}

void ProcessCORD(unsigned short session_id, unsigned char *data, long data_length, FILE *fout, bool client_to_server)
{
    WestwoodRC4 crypt;

    //CordHeader *header = (CordHeader *) data;

	//fprintf(fout, " %02X               CORD\n", data[0]);
    //fprintf(fout, " %02X %02X            Session ID\n", data[1], data[2]);
	//fprintf(fout, " %02X %02X            UDP Sequence Number\n", data[3], data[4]);
	//fprintf(fout, " %02X %02X %02X %02X      CRC-32\n", data[5], data[6], data[7], data[8]);

    hdiv = false;

    SessionInfo * session = g_SessionList;
	while (session)
	{
		if (session->session_id == session_id)
		{
			break;
		}
		session = session->next;
	}

	if (!session)
	{
		fprintf(fout, "ERROR: Unable to find matching session ID!\n");
		return;
	}

    session->rc4_key.udp_packet_length = data_length - 5;
    session->rc4_key.udp_sequence_number = data[3] * 256 + data[4];

    //fprintf(fout, "UDP Packet Length = %d\n", session->rc4_key.udp_packet_length);
    //fprintf(fout, "UDP Sequence Number = %d\n", session->rc4_key.udp_sequence_number);

    //fprintf(fout, "RC4 Key:\n");
    //DumpHex(session->rc4_key.session_key, 16, fout);

    //fprintf(fout, "Before RC4:\n");
    //DumpHex(data, data_length, fout);

    crypt.PrepareKey(session->rc4_key.session_key, 16);
    crypt.RC4(data + 5, data_length - 5);

    //fprintf(fout, "After RC4:\n");
    //DumpHex(data, data_length, fout);

    data += 9;
    data_length -= 9;

    unsigned char ctrl_1 = *data++; data_length--;
    unsigned char ctrl_2 = *data++; data_length--;
    //fprintf(fout, "-------\n");
    //fprintf(fout, " %02X %02X            CordMgr control data\n", ctrl_1, ctrl_2);

	if (ctrl_1 == 1)
	{
		int skip = 1;
		for (int n = 0; n < ctrl_2; n++)
		{
			if (data[skip + n * 4 + 1] & 0x80)
			{
				skip++;
			}
		}
		skip += ctrl_2 * 4;
        //DumpHex(data, skip, fout);
        //fprintf(fout, "-------\n");
        data += skip;
        data_length -= skip;
	}
	else if (((ctrl_1 == 2) || (ctrl_1 == 3)) && (data_length == 10))
	{
        //DumpHex(data, data_length, fout);
        //fprintf(fout, "-------\n");
        return;
	}
    else if ((ctrl_1 != 0) && (data_length <= 14))
    {
        fprintf(fout, "ERROR: Unable to interpret CordMgr header\n");
        DumpHex(data, data_length, fout);
        fprintf(fout, "-------\n");
        return;
    }

    ProcessCordMgrData(data, data_length, fout, client_to_server);
    //fprintf(fout, "\n");
}

void ProcessCordMgrData(unsigned char *data, long data_length, FILE *fout, bool client_to_server)
{
    long length = *data;
    if (length & 0x80)
    {
        length = (length & 0x7f) << 8;  // was 0x0f
        length |= data[1];
        //fprintf(fout, " %02X %02X            CordMgr data length = %d\n", data[0], data[1], length);
        data++;
        data_length--;
    }
    else
    {
    	//fprintf(fout, " %02X               CordMgr data length = %d\n", data[0], length);
    }
    data++;
    data_length--;

    unsigned short msg_type = *data;
    if (msg_type & 0x80)
    {
        msg_type = (msg_type & 0x7f) << 8;
        msg_type |= data[1];
        //fprintf(fout, " %02X %02X            CordMgr msg type\n", data[0], data[1]);
        data++;
        data_length--;
    }
    else
    {
    	//fprintf(fout, " %02X               CordMgr msg type\n", data[0]);
    }
    data++;
    data_length--;

    unsigned short msg_type2 = *data;
    if (msg_type2 & 0x80)
    {
        msg_type2 = (msg_type2 & 0x7f) << 8;
        msg_type2 |= data[1];
        //fprintf(fout, " %02X %02X            CordMgr msg type 2\n", data[0], data[1]);
        data++;
        data_length--;
    }
    else
    {
    	//fprintf(fout, " %02X               CordMgr msg type 2\n", data[0]);
    }
    data++;
    data_length--;

    short sequence_id = *data;
    //fprintf(fout, " %02X               CordMgr sequence ID\n", data[0]);
    data++;
    data_length--;

    unsigned char chunk_type = *data;
    //fprintf(fout, " %02X               CordMgr chunk type\n", data[0]);
    data++;
    data_length--;

    unsigned char chunk_index = 0;
    if ((chunk_type & 0x11) == 0x11)
    {
        chunk_index = *data;
        //fprintf(fout, " %02X               CordMgr chunk index\n", data[0]);
        data++;
        data_length--;
    }

	if (data_length == 0)
	{
		return;
	}

	if ((msg_type == 0x00) && (chunk_type == 0x05) && (data_length >= 5))
	{
        //fprintf(fout, "-------\n");
        ProcessCordMgrData(data, data_length, fout, client_to_server);
		return;
	}

	if (data_length == 0)
	{
		return;
	}

    // name change code goes here
    // BEGIN
    // END

    //fprintf(fout, "-------\n");

    long chunk_length = data_length;
    if (chunk_length > 200)
    {
        chunk_length = 200;
    }
    if (length < chunk_length)
    {
        chunk_length = length;
    }

    if ((chunk_type & 0x11) == 0x11)
    {
		if ((chunk_index + 1) * 200 > length)
		{
			chunk_length = length - chunk_index * 200;
			if (chunk_length <= 0)
			{
				if ((length > 0) && (data_length > 0))
				{
					fprintf(fout, "ERROR: Invalid chunk index\n");
					DumpHex(data, data_length, fout);
				}
				return;
			}
		}
		bytes_remaining = length - (chunk_index * 200);
        if ((chunk_index > 0) && (bytes_remaining > 0))
        {
            if (bytes_remaining < chunk_length)
            {
				if (!client_to_server)
				{
					CaptureAuxData(data, bytes_remaining);
				}

                // Finish off this chunk and continue processing the
                // next one in this packet
                DumpHex(data, bytes_remaining, fout);

                if (g_OpcodeDataIndex)
                {
                    memcpy(g_OpcodeData + g_OpcodeDataIndex, data, bytes_remaining);
                    g_OpcodeDataIndex += bytes_remaining;
                    if (g_OpcodeDataIndex >= g_OpcodeDataLength)
                    {
                        ProcessFullOpcodeData(g_OpcodeData, g_OpcodeDataLength);
                        g_OpcodeDataIndex = 0;
                        g_OpcodeDataLength = 0;
                    }
                }

                data += bytes_remaining;
                data_length -= bytes_remaining;
                bytes_remaining = 0;
                // recurse
                //fprintf(fout, "-------\n");
                ProcessCordMgrData(data, data_length, fout, client_to_server);
            }
            else if (chunk_length == data_length)
            {
                bytes_remaining -= chunk_length;
				if (!client_to_server)
				{
					CaptureAuxData(data, chunk_length);
				}

                DumpHex(data, chunk_length, fout);

                if (g_OpcodeDataIndex)
                {
                    memcpy(g_OpcodeData + g_OpcodeDataIndex, data, chunk_length);
                    g_OpcodeDataIndex += chunk_length;
                    if (g_OpcodeDataIndex >= g_OpcodeDataLength)
                    {
                        ProcessFullOpcodeData(g_OpcodeData, g_OpcodeDataLength);
                        g_OpcodeDataIndex = 0;
                        g_OpcodeDataLength = 0;
                    }
                }
            }
            else
            {
                bytes_remaining -= chunk_length;
				if (!client_to_server)
				{
					CaptureAuxData(data, chunk_length);
				}

                DumpHex(data, chunk_length, fout);

                if (g_OpcodeDataIndex)
                {
                    memcpy(g_OpcodeData + g_OpcodeDataIndex, data, chunk_length);
                    g_OpcodeDataIndex += chunk_length;
                    if (g_OpcodeDataIndex >= g_OpcodeDataLength)
                    {
                        ProcessFullOpcodeData(g_OpcodeData, g_OpcodeDataLength);
                        g_OpcodeDataIndex = 0;
                        g_OpcodeDataLength = 0;
                    }
                }

                // recurse
                //fprintf(fout, "-------\n");
                data += chunk_length;
                data_length -= chunk_length;
                ProcessCordMgrData(data, data_length, fout, client_to_server);
                return;
            }
            if (bytes_remaining < 0)
            {
                fprintf(fout, "ERROR: bytes_remaining < 0!\n");
            }
            return;
        }

        // chunk 0 - determine bytes remaining
        bytes_remaining = length - chunk_length;
        //fprintf(fout, "Bytes remaining = %d\n", bytes_remaining);
        if (bytes_remaining < 0)
        {
            fprintf(fout, "ERROR: bytes_remaining < 0!\n");
        }
    }

	bool found_opcode = false;
    if (length == 0)
    {
        ProcessCordMgrData(data, data_length, fout, client_to_server);
    }
    else if (data_length > chunk_length)
    {
		if (chunk_index == 0)
		{
			found_opcode = ProcessOpcode(data, chunk_length, fout, client_to_server);
		}
		if (!client_to_server)
		{
			CaptureAuxData(data, chunk_length);
		}
		if (found_opcode)
		{
			data += 4;
			data_length -= 4;
			chunk_length -= 4;
		}

        DumpHex(data, chunk_length, fout);

        data += chunk_length;
        data_length -= chunk_length;
        // recurse
        //fprintf(fout, "-------\n");
        ProcessCordMgrData(data, data_length, fout, client_to_server);
    }
    else
    {
		if (chunk_index == 0)
		{
			found_opcode = ProcessOpcode(data, data_length, fout, client_to_server);
		}
		if (!client_to_server)
		{
			CaptureAuxData(data, data_length);
		}
		if (found_opcode)
		{
			data += 4;
			data_length -= 4;
		}

        DumpHex(data, data_length, fout);
    }
}

bool ProcessOpcode(unsigned char *data, long data_length, FILE *fout, bool client_to_server)
{
	bool success = false;
	unsigned short length = data[0] + data[1] * 256;
	unsigned short opcode = data[2] + data[3] * 256;
	if (data_length >= 4)
	{
		if (opcode < NUM_OPCODES)
		{
			g_opcode[opcode].count++;
			g_opcode[opcode].accumulated_size += length;
			if (client_to_server)
			{
				g_opcode[opcode].c2s_count++;
			}
			else
			{
				g_opcode[opcode].s2c_count++;
			}
			if (length > g_opcode[opcode].max_size)
			{
				g_opcode[opcode].max_size = length;
			}
			if ((g_opcode[opcode].min_size == 0) ||
				(length < g_opcode[opcode].min_size))
			{
				g_opcode[opcode].min_size = length;
			}
            if (hdiv)
            {
                fprintf(fout, "\n--------------------\n\n");
            }
            hdiv = true;
			fprintf(fout, " %02X %02X            Length = %d bytes\n", data[0], data[1], length);
			fprintf(fout, " %02X %02X            Opcode 0x%02X = %s\n", data[2], data[3], opcode, opcode_string[opcode]);
			success = true;

            // If this is the Item_Base opcode, dump it in XML format
            if (length > data_length)
            {
                memcpy(g_OpcodeData, data, data_length);
                g_OpcodeDataIndex = data_length;
                g_OpcodeDataLength = length;
            }
            else
            {
                ProcessFullOpcodeData(data, data_length);
            }
		}
		else
		{
			fprintf(fout, "ERROR: Unrecognized opcode %04x\n", opcode);
		}
	}

	return success;
}

// Fast CRC16 routine
WORD CRC16(BYTE *msg, int numBytes)
{
	static const BYTE CRC_Lo[256] = {
		0x00, 0x21, 0x42, 0x63, 0x84, 0xa5, 0xc6, 0xe7,
		0x08, 0x29, 0x4a, 0x6b, 0x8c, 0xad, 0xce, 0xef,
		0x31, 0x10, 0x73, 0x52, 0xb5, 0x94, 0xf7, 0xd6,
		0x39, 0x18, 0x7b, 0x5a, 0xbd, 0x9c, 0xff, 0xde,
		0x62, 0x43, 0x20, 0x01, 0xe6, 0xc7, 0xa4, 0x85,
		0x6a, 0x4b, 0x28, 0x09, 0xee, 0xcf, 0xac, 0x8d,
		0x53, 0x72, 0x11, 0x30, 0xd7, 0xf6, 0x95, 0xb4,
		0x5b, 0x7a, 0x19, 0x38, 0xdf, 0xfe, 0x9d, 0xbc,
		0xc4, 0xe5, 0x86, 0xa7, 0x40, 0x61, 0x02, 0x23,
		0xcc, 0xed, 0x8e, 0xaf, 0x48, 0x69, 0x0a, 0x2b,
		0xf5, 0xd4, 0xb7, 0x96, 0x71, 0x50, 0x33, 0x12,
		0xfd, 0xdc, 0xbf, 0x9e, 0x79, 0x58, 0x3b, 0x1a,
		0xa6, 0x87, 0xe4, 0xc5, 0x22, 0x03, 0x60, 0x41,
		0xae, 0x8f, 0xec, 0xcd, 0x2a, 0x0b, 0x68, 0x49,
		0x97, 0xb6, 0xd5, 0xf4, 0x13, 0x32, 0x51, 0x70,
		0x9f, 0xbe, 0xdd, 0xfc, 0x1b, 0x3a, 0x59, 0x78,
		0x88, 0xa9, 0xca, 0xeb, 0x0c, 0x2d, 0x4e, 0x6f,
		0x80, 0xa1, 0xc2, 0xe3, 0x04, 0x25, 0x46, 0x67,
		0xb9, 0x98, 0xfb, 0xda, 0x3d, 0x1c, 0x7f, 0x5e,
		0xb1, 0x90, 0xf3, 0xd2, 0x35, 0x14, 0x77, 0x56,
		0xea, 0xcb, 0xa8, 0x89, 0x6e, 0x4f, 0x2c, 0x0d,
		0xe2, 0xc3, 0xa0, 0x81, 0x66, 0x47, 0x24, 0x05,
		0xdb, 0xfa, 0x99, 0xb8, 0x5f, 0x7e, 0x1d, 0x3c,
		0xd3, 0xf2, 0x91, 0xb0, 0x57, 0x76, 0x15, 0x34,
		0x4c, 0x6d, 0x0e, 0x2f, 0xc8, 0xe9, 0x8a, 0xab,
		0x44, 0x65, 0x06, 0x27, 0xc0, 0xe1, 0x82, 0xa3,
		0x7d, 0x5c, 0x3f, 0x1e, 0xf9, 0xd8, 0xbb, 0x9a,
		0x75, 0x54, 0x37, 0x16, 0xf1, 0xd0, 0xb3, 0x92,
		0x2e, 0x0f, 0x6c, 0x4d, 0xaa, 0x8b, 0xe8, 0xc9,
		0x26, 0x07, 0x64, 0x45, 0xa2, 0x83, 0xe0, 0xc1,
		0x1f, 0x3e, 0x5d, 0x7c, 0x9b, 0xba, 0xd9, 0xf8,
		0x17, 0x36, 0x55, 0x74, 0x93, 0xb2, 0xd1, 0xf0
	};

	static const BYTE CRC_Hi[256] = {
		0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
		0x81, 0x91, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1,
		0x12, 0x02, 0x32, 0x22, 0x52, 0x42, 0x72, 0x62,
		0x93, 0x83, 0xb3, 0xa3, 0xd3, 0xc3, 0xf3, 0xe3,
		0x24, 0x34, 0x04, 0x14, 0x64, 0x74, 0x44, 0x54,
		0xa5, 0xb5, 0x85, 0x95, 0xe5, 0xf5, 0xc5, 0xd5,
		0x36, 0x26, 0x16, 0x06, 0x76, 0x66, 0x56, 0x46,
		0xb7, 0xa7, 0x97, 0x87, 0xf7, 0xe7, 0xd7, 0xc7,
		0x48, 0x58, 0x68, 0x78, 0x08, 0x18, 0x28, 0x38,
		0xc9, 0xd9, 0xe9, 0xf9, 0x89, 0x99, 0xa9, 0xb9,
		0x5a, 0x4a, 0x7a, 0x6a, 0x1a, 0x0a, 0x3a, 0x2a,
		0xdb, 0xcb, 0xfb, 0xeb, 0x9b, 0x8b, 0xbb, 0xab,
		0x6c, 0x7c, 0x4c, 0x5c, 0x2c, 0x3c, 0x0c, 0x1c,
		0xed, 0xfd, 0xcd, 0xdd, 0xad, 0xbd, 0x8d, 0x9d,
		0x7e, 0x6e, 0x5e, 0x4e, 0x3e, 0x2e, 0x1e, 0x0e,
		0xff, 0xef, 0xdf, 0xcf, 0xbf, 0xaf, 0x9f, 0x8f,
		0x91, 0x81, 0xb1, 0xa1, 0xd1, 0xc1, 0xf1, 0xe1,
		0x10, 0x00, 0x30, 0x20, 0x50, 0x40, 0x70, 0x60,
		0x83, 0x93, 0xa3, 0xb3, 0xc3, 0xd3, 0xe3, 0xf3,
		0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72,
		0xb5, 0xa5, 0x95, 0x85, 0xf5, 0xe5, 0xd5, 0xc5,
		0x34, 0x24, 0x14, 0x04, 0x74, 0x64, 0x54, 0x44,
		0xa7, 0xb7, 0x87, 0x97, 0xe7, 0xf7, 0xc7, 0xd7,
		0x26, 0x36, 0x06, 0x16, 0x66, 0x76, 0x46, 0x56,
		0xd9, 0xc9, 0xf9, 0xe9, 0x99, 0x89, 0xb9, 0xa9,
		0x58, 0x48, 0x78, 0x68, 0x18, 0x08, 0x38, 0x28,
		0xcb, 0xdb, 0xeb, 0xfb, 0x8b, 0x9b, 0xab, 0xbb,
		0x4a, 0x5a, 0x6a, 0x7a, 0x0a, 0x1a, 0x2a, 0x3a,
		0xfd, 0xed, 0xdd, 0xcd, 0xbd, 0xad, 0x9d, 0x8d,
		0x7c, 0x6c, 0x5c, 0x4c, 0x3c, 0x2c, 0x1c, 0x0c,
		0xef, 0xff, 0xcf, 0xdf, 0xaf, 0xbf, 0x8f, 0x9f,
		0x6e, 0x7e, 0x4e, 0x5e, 0x2e, 0x3e, 0x0e, 0x1e
	};
	int i, index;
	union {
		WORD w;
		BYTE b[2];
	} crc;

	crc.w = 0;

	for (i = 0; i < numBytes; i++)
	{
        index = crc.b[1] ^ *msg++;
        crc.b[1] = crc.b[0] ^ CRC_Hi[index];
        crc.b[0] = CRC_Lo[index];
	}

	return (crc.w);
}

// Free the memory used by the Sector Content data
void Cleanup()
{
    for (int i = 0; i < g_SectorContentCount; i++)
    {
        if (g_SectorContentArray[i].CreatePacket)
        {
            delete g_SectorContentArray[i].CreatePacket;
            g_SectorContentArray[i].CreatePacket = NULL;
        }
        if (g_SectorContentArray[i].ActivateRenderStatePackets[0])
        {
            delete g_SectorContentArray[i].ActivateRenderStatePackets[0];
            g_SectorContentArray[i].ActivateRenderStatePackets[0] = NULL;
        }
        if (g_SectorContentArray[i].ActivateRenderStatePackets[1])
        {
            delete g_SectorContentArray[i].ActivateRenderStatePackets[1];
            g_SectorContentArray[i].ActivateRenderStatePackets[1] = NULL;
        }
        if (g_SectorContentArray[i].RelationshipPacket)
        {
            delete g_SectorContentArray[i].RelationshipPacket;
            g_SectorContentArray[i].RelationshipPacket = NULL;
        }
        if (g_SectorContentArray[i].SimplePositionalUpdate)
        {
            delete g_SectorContentArray[i].SimplePositionalUpdate;
            g_SectorContentArray[i].SimplePositionalUpdate = NULL;
        }
        if (g_SectorContentArray[i].AdvancedPositionalUpdate)
        {
            delete g_SectorContentArray[i].AdvancedPositionalUpdate;
            g_SectorContentArray[i].AdvancedPositionalUpdate = NULL;
        }
        if (g_SectorContentArray[i].PlanetPositionalUpdate)
        {
            delete g_SectorContentArray[i].PlanetPositionalUpdate;
            g_SectorContentArray[i].PlanetPositionalUpdate = NULL;
        }
        if (g_SectorContentArray[i].ConstantPositionalUpdate)
        {
            delete g_SectorContentArray[i].ConstantPositionalUpdate;
            g_SectorContentArray[i].ConstantPositionalUpdate = NULL;
        }
        if (g_SectorContentArray[i].ComponentPositionalUpdate)
        {
            delete g_SectorContentArray[i].ComponentPositionalUpdate;
            g_SectorContentArray[i].ComponentPositionalUpdate = NULL;
        }
        if (g_SectorContentArray[i].AuxDataPackets[0])
        {
            delete g_SectorContentArray[i].AuxDataPackets[0];
            g_SectorContentArray[i].AuxDataPackets[0] = NULL;
        }
        if (g_SectorContentArray[i].AuxDataPackets[1])
        {
            delete g_SectorContentArray[i].AuxDataPackets[1];
            g_SectorContentArray[i].AuxDataPackets[1] = NULL;
        }
        if (g_SectorContentArray[i].AuxDataPackets[2])
        {
            delete g_SectorContentArray[i].AuxDataPackets[2];
            g_SectorContentArray[i].AuxDataPackets[2] = NULL;
        }
        if (g_SectorContentArray[i].NavigationPacket)
        {
            delete g_SectorContentArray[i].NavigationPacket;
            g_SectorContentArray[i].NavigationPacket = NULL;
        }
        for (int j = 0; j < 3; j++)
        {
            if (g_SectorContentArray[i].ObjectEffectPackets[j])
            {
                delete g_SectorContentArray[i].ObjectEffectPackets[j];
                g_SectorContentArray[i].ObjectEffectPackets[j] = NULL;
            }
        }
    }
}

void ProcessLoginOpcode(unsigned char *data, long data_length)
{
    long sector_id = ntohl(*((long *) &data[24]));
    if (g_SectorID != sector_id)
    {
        g_SectorID = sector_id;
        g_SectorContent = NULL;
        for (int i = 0; i < g_NumSectors; i++)
        {
            if (g_SectorList[i] == sector_id)
            {
                printf("SectorID = %d (duplicate)\n", sector_id);
                return;
            }
        }
        g_SectorList[g_NumSectors++] = sector_id;
        printf("SectorID = %d (new!)\n", sector_id);
    }
}

void ProcessCreateOpcode(unsigned char *data, long data_length)
{
    long game_id = *((long *) &data[4]);
    if (game_id <= 0)
    {
        // Ignore Create with game_id of 0 or -1
        g_SectorContent = NULL;
        return;
    }

    g_TotalCreateOpcodes++;

    short basset = *((short *) &data[12]);
    if ((basset >= 1600) && (basset <= 1626))
    {
        // This is a hull - ignore the create ship opcodes
        return;
    }

    // First loop through the existing sector content to see if this is a duplicate
    for (int i = 0; i < g_SectorContentCount; i++)
    {
        if ((g_SectorContentArray[i].GameID == game_id) &&
            (g_SectorContentArray[i].SectorID == g_SectorID))
        {
            // This is a duplicate, ignore it
            g_SectorContentArray[i].IgnoreDuplicate = 1;
            g_SectorContent = NULL;
            return;
        }
    }

    // This is the first time we have seen this object
    // Create a new entry in the array
    g_SectorContent = &g_SectorContentArray[g_SectorContentCount++];
    g_SectorContent->GameID = game_id;
    g_SectorContent->SectorID = g_SectorID;

    // Make a copy of the create packet
    g_SectorContent->CreatePacket = new BYTE[data_length];
    memcpy(g_SectorContent->CreatePacket, data, data_length);
}

void ProcessActivateRenderStateOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[4]);
        if (g_SectorContent->GameID == game_id)
        {
            if (!g_SectorContent->ActivateRenderStatePackets[0])
            {
                // Make a copy of the ActivateRenderState packet
                g_SectorContent->ActivateRenderStatePackets[0] = new BYTE[data_length];
                memcpy(g_SectorContent->ActivateRenderStatePackets[0], data, data_length);
            }
            else if (!g_SectorContent->ActivateRenderStatePackets[1])
            {
                // Make a copy of the ActivateRenderState packet
                g_SectorContent->ActivateRenderStatePackets[1] = new BYTE[data_length];
                memcpy(g_SectorContent->ActivateRenderStatePackets[1], data, data_length);
            }
            else
            {
                printf("ERROR: Sector content already contains two ActivateRenderState packets!\n");
            }
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessObjectEffectOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[5]); // data[4] = Bitmask
        if (g_SectorContent->GameID == game_id)
        {
            for (int i = 0; i < 3; i++)
            {
                if (!g_SectorContent->ObjectEffectPackets[i])
                {
                    // Make a copy of the ObjectEffect packet
                    g_SectorContent->ObjectEffectPackets[i] = new BYTE[data_length];
                    memcpy(g_SectorContent->ObjectEffectPackets[i], data, data_length);
                    return;
                }
            }

            printf("ERROR: Sector content already contains three ObjectEffect packets!\n");
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessRelationshipOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = ntohl(*((long *) &data[4])); // oddball Big Endian
        if (g_SectorContent->GameID == game_id)
        {
            if (!g_SectorContent->RelationshipPacket)
            {
                // Make a copy of the Relationship packet
                g_SectorContent->RelationshipPacket = new BYTE[data_length];
                memcpy(g_SectorContent->RelationshipPacket, data, data_length);
                return;
            }
            else
            {
                printf("ERROR: Sector content already contains a Relationship packet!\n");
            }
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessSimplePositionalUpdateOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[4]);
        if (g_SectorContent->GameID == game_id)
        {
            if (!g_SectorContent->SimplePositionalUpdate)
            {
                // Make a copy of the SimplePositionalUpdate packet
                g_SectorContent->SimplePositionalUpdate = new BYTE[data_length];
                memcpy(g_SectorContent->SimplePositionalUpdate, data, data_length);
                return;
            }
            else
            {
                printf("ERROR: Sector content already contains a SimplePositionalUpdate packet!\n");
            }
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessAdvancedPositionalUpdateOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[6]);    // Skip 2-byte bitmask
        if (g_SectorContent->GameID == game_id)
        {
            if (!g_SectorContent->AdvancedPositionalUpdate)
            {
                // Make a copy of the AdvancedPositionalUpdate packet
                g_SectorContent->AdvancedPositionalUpdate = new BYTE[data_length];
                memcpy(g_SectorContent->AdvancedPositionalUpdate, data, data_length);
            }
            else
            {
                //printf("WARNING: Sector content already contains a AdvancedPositionalUpdate packet!\n");
                delete g_SectorContent->AdvancedPositionalUpdate;
                g_SectorContent->AdvancedPositionalUpdate = new BYTE[data_length];
                memcpy(g_SectorContent->AdvancedPositionalUpdate, data, data_length);
            }
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessPlanetPositionalUpdateOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[4]);
        if (g_SectorContent->GameID == game_id)
        {
            if (!g_SectorContent->PlanetPositionalUpdate)
            {
                // Make a copy of the PlanetPositionalUpdate packet
                g_SectorContent->PlanetPositionalUpdate = new BYTE[data_length];
                memcpy(g_SectorContent->PlanetPositionalUpdate, data, data_length);
                return;
            }
            else
            {
                printf("ERROR: Sector content already contains a PlanetPositionalUpdate packet!\n");
            }
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessConstantPositionalUpdateOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[4]);
        if (g_SectorContent->GameID == game_id)
        {
            if (!g_SectorContent->ConstantPositionalUpdate)
            {
                // Make a copy of the ConstantPositionalUpdate packet
                g_SectorContent->ConstantPositionalUpdate = new BYTE[data_length];
                memcpy(g_SectorContent->ConstantPositionalUpdate, data, data_length);
                return;
            }
            else
            {
                printf("ERROR: Sector content already contains a ConstantPositionalUpdate packet!\n");
            }
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessComponentPositionalUpdateOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[4]);
        if (g_SectorContent->GameID == game_id)
        {
            if (!g_SectorContent->ComponentPositionalUpdate)
            {
                // Make a copy of the ComponentPositionalUpdate packet
                g_SectorContent->ComponentPositionalUpdate = new BYTE[data_length];
                memcpy(g_SectorContent->ComponentPositionalUpdate, data, data_length);
                return;
            }
            else
            {
                printf("ERROR: Sector content already contains a ComponentPositionalUpdate packet!\n");
            }
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessAuxDataOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[4]);
        if (g_SectorContent->GameID == game_id)
        {
            if (!g_SectorContent->AuxDataPackets[0])
            {
                // Make a copy of the AuxData packet
                g_SectorContent->AuxDataPackets[0] = new BYTE[data_length];
                memcpy(g_SectorContent->AuxDataPackets[0], data, data_length);
            }
            else if (!g_SectorContent->AuxDataPackets[1])
            {
                // Make a copy of the AuxData packet
                g_SectorContent->AuxDataPackets[1] = new BYTE[data_length];
                memcpy(g_SectorContent->AuxDataPackets[1], data, data_length);
            }
            else if (!g_SectorContent->AuxDataPackets[2])
            {
                // Make a copy of the AuxData packet
                g_SectorContent->AuxDataPackets[2] = new BYTE[data_length];
                memcpy(g_SectorContent->AuxDataPackets[2], data, data_length);
            }
            else
            {
                printf("ERROR: Sector content already contains three AuxData packets!\n");
            }
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessNavigationOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[4]);
        if (g_SectorContent->GameID == game_id)
        {
            if (!g_SectorContent->NavigationPacket)
            {
                // Make a copy of the Navigation packet
                g_SectorContent->NavigationPacket = new BYTE[data_length];
                memcpy(g_SectorContent->NavigationPacket, data, data_length);
                return;
            }
            else
            {
                printf("ERROR: Sector content already contains a Navigation packet!\n");
            }
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessRemoveOpcode(unsigned char *data, long data_length)
{
    if ((g_SectorContent) &&
        (g_SectorContent->SectorID == g_SectorID) &&
        (!g_SectorContent->IgnoreDuplicate))
    {
        long game_id = *((long *) &data[4]);
        if (g_SectorContent->GameID == game_id)
        {
            g_SectorContent->Removed = 1;
        }
        else
        {
            // Not processing the same GameID any longer
            g_SectorContent = NULL;
        }
    }
}

void ProcessFullOpcodeData(unsigned char *data, long data_length)
{
    short opcode = *((short *) &data[2]);

    switch (opcode)
    {
    case ENB_OPCODE_0002_LOGIN : 
        ProcessLoginOpcode(data, data_length);  // determine what sector we are logging into
        break;

    case ENB_OPCODE_0004_CREATE : 
        ProcessCreateOpcode(data, data_length);
        break;

    case ENB_OPCODE_0007_REMOVE :
        ProcessRemoveOpcode(data, data_length);
        break;

    case ENB_OPCODE_0008_SIMPLE_POSITIONAL_UDPATE :
        ProcessSimplePositionalUpdateOpcode(data, data_length);
        break;

    case ENB_OPCODE_0009_OBJECT_EFFECT :
        ProcessObjectEffectOpcode(data, data_length);
        break;

    case ENB_OPCODE_0025_ITEM_BASE :
        DumpItemBase(data, data_length);
        break;

    case ENB_OPCODE_0030_ACTIVATE_RENDER_STATE :
        ProcessActivateRenderStateOpcode(data, data_length);
        break;

    case ENB_OPCODE_003E_ADVANCED_POSITIONAL_UPDATE :
        ProcessAdvancedPositionalUpdateOpcode(data, data_length);
        break;

    case ENB_OPCODE_003F_PLANET_POSITIONAL_UPDATE :
        ProcessPlanetPositionalUpdateOpcode(data, data_length);
        break;

    case ENB_OPCODE_0040_CONSTANT_POSITIONAL_UPDATE :
        ProcessConstantPositionalUpdateOpcode(data, data_length);
        break;

    case ENB_OPCODE_0046_COMPONENT_POSITIONAL_UPDATE :
        ProcessComponentPositionalUpdateOpcode(data, data_length);
        break;

    case ENB_OPCODE_001B_AUX_DATA :
        ProcessAuxDataOpcode(data, data_length);
        break;

    case ENB_OPCODE_0089_RELATIONSHIP :
        ProcessRelationshipOpcode(data, data_length);
        break;

    case ENB_OPCODE_0099_NAVIGATION :
        ProcessNavigationOpcode(data, data_length);
        break;

    default :
        break;
    }
}

void DumpItemBase(unsigned char *data, long data_length)
{
    int i, j;

    if (!item_base_file)
    {
        return;
    }

    // Count the total number of Item_Base packets processed
    g_NumItemBasePackets++;

    // First see if this item has already been dumped to the XML file.
    long ItemTemplateID = ntohl(*((long *) &data[4]));

    for (i=0; i<g_NumItems; i++)
    {
        if (ItemTemplateID == g_ItemTemplateID[i])
        {
            // duplicate
            return;
        }
    }

    // Add this to the list of unique Item_Base packets
    g_ItemTemplateID[g_NumItems++] = ItemTemplateID;

    long * plong = (long *) &data[8];
    long Category = *plong++;
    long Subcategory = *plong++;
    long ItemType = *plong++;

    //fprintf(item_base_file, "ItemTemplateID = 0x%08x\n", ItemTemplateID);
    //fprintf(item_base_file, "Category = 0x%08x\n", Category);
    //fprintf(item_base_file, "Subcategory = 0x%08x\n", Subcategory);
    //fprintf(item_base_file, "ItemType = 0x%08x\n", ItemType);

    unsigned char *p = (unsigned char *) plong;

    int ItemFieldCount = *p++;

    //fprintf(item_base_file, "ItemFieldCount = %d\n", ItemFieldCount);

    long *ItemFieldData = (long *) p;

    short string_length;
    long field_id;

    // For now, we will just skip over the Item Fields
    // to find the next section
    for (i = 0; i < ItemFieldCount; i++)
    {
        field_id = *((long *) p);
        p += 4; // skip the field id

        //fprintf(item_base_file, "Item #%d FieldID = 0x%08X\n", i, field_id);

        switch (field_id)
        {
        case 0x01 : // #1 = unknown short string
        case 0x0B : // #11 = Tech Type short string
        case 0x0D : // #13 = Item Level short string
        case 0x1B : // #27 = unknown short string
            string_length = *((short *) p);
            p += 2;
            //fprintf(item_base_file, "short string: %d bytes \"%.*s\"\n", string_length, string_length, p);
            p += string_length; // skip the string
            break;

        default :   // everything else appears to have a 4-byte argument
            //fprintf(item_base_file, "data = 0x%08x\n", *((long *) p));
            p += 4; // skip the data
            break;
        }
    }

    long ActivatableEffectCount = ntohl(*((long *) p));
    p += 4;
    char *ActivatableEffectData = (char *) p;

    //fprintf(item_base_file, "ActivatableEffectCount = %d\n", ActivatableEffectCount);

    if (ActivatableEffectCount < 0)
    {
        fprintf(item_base_file, "ERROR: ActivatableEffectCount = %d is negative!\n", ActivatableEffectCount);
        return;
    }

    if (ActivatableEffectCount > 6)
    {
        fprintf(item_base_file, "ERROR: ActivatableEffectCount = %d seems high\n", ActivatableEffectCount);
        return;
    }

    for (i = 0; i < ActivatableEffectCount; i++)
    {
        // skip the name
        string_length = *((short *) p);
        p += 2;
        //fprintf(item_base_file, "Name = \"%.*s\"\n", string_length, p);
        p += string_length; // skip the string

        // skip the description
        string_length = *((short *) p);
        p += 2;
        //fprintf(item_base_file, "Description = \"%.*s\"\n", string_length, p);
        p += string_length; // skip the string

        // skip the tooltip
        string_length = *((short *) p);
        p += 2;
        //fprintf(item_base_file, "Tooltip = \"%.*s\"\n", string_length, p);
        p += string_length; // skip the string

        long ActivatableLongNameVariableCount = ntohl(*((long *) p));
        p += 4;
        //fprintf(item_base_file, "ActivatableLongNameVariableCount = %d\n", ActivatableLongNameVariableCount);

        if (ActivatableLongNameVariableCount != 0)
        {
            //fprintf(item_base_file, "ActivatableLongNameVariableCount is non zero!\n");
            return;
        }

        long ActivatableDescriptionVariableCount = ntohl(*((long *)p));
        p += 4;
        //fprintf(item_base_file, "ActivatableDescriptionVariableCount = %d\n", ActivatableDescriptionVariableCount);

        long * ActivatableDescriptionVariableData = (long *) p;

        // Skip the description variables
        for (j = 0; j < ActivatableDescriptionVariableCount; j++)
        {
            p += 4;
        }

        long ActivatableEffectFlag1 = *((long *) p);
        p += 4;
        //fprintf(item_base_file, "ActivatableEffectFlag1 = 0x%08x\n", ActivatableEffectFlag1);

        long ActivatableEffectFlag2 = *((long *) p);
        p += 4;
        ///fprintf(item_base_file, "ActivatableEffectFlag2 = 0x%08x\n", ActivatableEffectFlag2);
    }

    long * ActivatableUnknownData = NULL;
    if (ActivatableEffectCount > 0)
    {
        // Skip the four unknown values for now
        ActivatableUnknownData = (long *) p;
        p += 16;
    }

    // Now process the Equipable Effects
    long EquipableEffectCount = ntohl(*((long *) p));
    p += 4;
    char *EquipableEffectData = (char *) p;

    //fprintf(item_base_file, "EquipableEffectCount = %d\n", EquipableEffectCount);

    if (EquipableEffectCount < 0)
    {
        fprintf(item_base_file, "ERROR: EquipableEffectCount = %d is negative!\n", EquipableEffectCount);
        return;
    }

    if (EquipableEffectCount > 6)
    {
        fprintf(item_base_file, "ERROR: EquipableEffectCount = %d seems high\n", EquipableEffectCount);
        return;
    }

    for (i = 0; i < EquipableEffectCount; i++)
    {
        // skip the name
        string_length = *((short *) p);
        p += 2;
        //fprintf(item_base_file, "Name = \"%.*s\"\n", string_length, p);
        p += string_length; // skip the string

        // skip the description
        string_length = *((short *) p);
        p += 2;
        //fprintf(item_base_file, "Description = \"%.*s\"\n", string_length, p);
        p += string_length; // skip the string

        // skip the tooltip
        string_length = *((short *) p);
        p += 2;
        //fprintf(item_base_file, "Tooltip = \"%.*s\"\n", string_length, p);
        p += string_length; // skip the string

        long EquipableLongNameVariableCount = ntohl(*((long *) p));
        p += 4;
        if (EquipableLongNameVariableCount != 0)
        {
            fprintf(item_base_file, "ERROR: EquipableLongNameVariableCount is non zero!\n");
            return;
        } 

        long EquipableDescriptionVariableCount = ntohl(*((long *) p));
        p += 4;
        long * EquipableDescriptionVariableData = (long *) p;

        // Skip the description variables
        for (j = 0; j < EquipableDescriptionVariableCount; j++)
        {
            p += 4;
        }

        long EquipableEffectFlag1 = *((long *) p);
        p += 4;
        long EquipableEffectFlag2 = *((long *) p);
        p += 4;
    }

    long * EquipableUnknownData = NULL;
    if (EquipableEffectCount)
    {
        // Skip the four unknown values for now
        EquipableUnknownData = (long *) p;
        p += 16;
    }

    short GameBasset = ntohs(*((short *) p)); p += 2;
    short IconBaseAsset = ntohs(*((short *) p)); p += 2;
    short TechLevel = ntohs(*((short *) p)); p += 2;
    long  Cost = ntohl(*((long *) p)); p += 4;
    long  MaxStack = ntohl(*((long *) p)); p += 4;
    long  EffectUsage = ntohl(*((long *) p)); p += 4;
    long  Flags = *((long *) p); p += 4;

    string_length = *((short *) p);
    p += 2;

    char *name = (char *) p;
    short name_length = string_length;
    //fprintf(item_base_file, "Name = \"%.*s\"\n", string_length, p);
    p += string_length;

    string_length = *((short *) p);
    p += 2;

    char *description = (char *) p;
    short description_length = string_length;
    //fprintf(item_base_file, "Description = \"%.*s\"\n", string_length, p);
    p += string_length;

    string_length = *((short *) p);
    p += 2;

    char *manufacturer = (char *) p;
    short manufacturer_length = string_length;
    //fprintf(item_base_file, "Manufacturer = \"%.*s\"\n", string_length, p);
    p += string_length;

    if (p != &data[data_length])
    {
        fprintf(item_base_file, "ERROR: Parse does not match the data length!\n");
    }

    /////// csv format: fprintf(item_base_file, "%d,%.*s\n", ItemTemplateID, name_length, name);

    // Now that we are done parsing, we can begin printing the results in XML format
    //fprintf(item_base_file, "=======================================================\n");

    fprintf(item_base_file, "<ItemBase ItemTemplateID=\"%d\" Category=\"%d\" Subcategory=\"%d\" ItemType=\"%d\"\n",
                            ItemTemplateID, Category, Subcategory, ItemType);
    fprintf(item_base_file, "          GameBasset=\"%d\"  IconBaseAsset=\"%d\" TechLevel=\"%d\" Cost=\"%d\"\n",
                            GameBasset, IconBaseAsset, TechLevel, Cost);
    fprintf(item_base_file, "          MaxStack=\"%d\" EffectUsage=\"%d\" Flags=\"%d\">\n",
                            MaxStack, EffectUsage, Flags);
    fprintf(item_base_file, "    <Name>%.*s</Name>\n", name_length, name);
    fprintf(item_base_file, "    <Description>%.*s</Description>\n", description_length, description);
    if (manufacturer_length > 0)
    {
        fprintf(item_base_file, "    <Manufacturer>%.*s</Manufacturer>\n", manufacturer_length, manufacturer);
    }

    long iValue;
    float fValue;

    if (ItemFieldCount > 0)
    {
        fprintf(item_base_file, "    <ItemFields>\n");

        p = (unsigned char *) ItemFieldData;

        // Loop through all of the Item Fields and generate the XML
        for (i = 0; i < ItemFieldCount; i++)
        {
            field_id = *((long *) p);
            p += 4; // skip the field id
            switch (field_id)
            {
            // short string
            case 0x01 : // #1 = unknown short string
            case 0x0B : // #11 = Tech Type short string
            case 0x0D : // #13 = Item Level short string
            case 0x1B : // #27 = unknown short string
                string_length = *((short *) p);
                p += 2;
                fprintf(item_base_file, "        <Item type=\"%d\">%.*s</Item>\n",
                    field_id, string_length, p);
                p += string_length; // skip the string
                break;

            // 4-byte float
            case 0x00 :
            case 0x09 :
            case 0x0A :
            case 0x14 :
            case 0x15 :
            case 0x17 :
            case 0x19 :
            case 0x1A :
            case 0x22 :
            case 0x24 :
            case 0x25 :
                fValue = *((float *) p);
                p += 4;
                fprintf(item_base_file, "        <Item type=\"%d\">%g</Item>\n", field_id, fValue);
                break;

            default :
                iValue = *((long *) p);
                p += 4;
                fprintf(item_base_file, "        <Item type=\"%d\">%d</Item>\n", field_id, iValue);
                break;
            }
        }

        fprintf(item_base_file, "    </ItemFields>\n");
    }

    if (ActivatableEffectCount > 0)
    {
        fprintf(item_base_file, "    <ActivatableEffects unknown1=\"%d\" unknown2=\"%d\" unknown3=\"%d\" unknown4=\"%d\">\n",
            ntohl(ActivatableUnknownData[0]),
            ntohl(ActivatableUnknownData[1]),
            ntohl(ActivatableUnknownData[2]),
            ntohl(ActivatableUnknownData[3]));

        p = (unsigned char *) ActivatableEffectData;
        for (i = 0; i < ActivatableEffectCount; i++)
        {
            // skip the name
            string_length = *((short *) p);
            p += 2;
            char *name = (char *) p;
            int name_length = string_length;
            p += string_length; // skip the string

            // skip the description
            string_length = *((short *) p);
            p += 2;
            char *description = (char *) p;
            int description_length = string_length;
            p += string_length; // skip the string

            // skip the tooltip
            string_length = *((short *) p);
            p += 2;
            char *tooltip = (char *) p;
            int tooltip_length = string_length;
            p += string_length; // skip the string

            long ActivatableLongNameVariableCount = ntohl(*((long *) p));
            p += 4;

            long ActivatableDescriptionVariableCount = ntohl(*((long *)p));
            p += 4;

            long * ActivatableDescriptionVariableData = (long *) p;

            // Skip the description variables
            for (j = 0; j < ActivatableDescriptionVariableCount; j++)
            {
                p += 4;
            }

            long Flag1 = *((long *) p);
            p += 4;

            long Flag2 = *((long *) p);
            p += 4;

            fprintf(item_base_file, "        <Effect  flag1=\"%d\" flag2=\"%d\">\n", Flag1, Flag2);
            fprintf(item_base_file, "            <Name>%.*s</Name>\n", name_length, name);
            fprintf(item_base_file, "            <Description>%.*s</Description>\n", description_length, description);
            fprintf(item_base_file, "            <Tooltip>%.*s</Tooltip>\n", tooltip_length, tooltip);
            if (ActivatableDescriptionVariableCount > 0)
            {
                fprintf(item_base_file, "            <DescriptionVars>\n");
                for (j = 0; j < ActivatableDescriptionVariableCount; j++)
                {
                    // The float data is in network order
                    iValue = ntohl(ActivatableDescriptionVariableData[j]);
                    fValue = *((float *) &iValue);
                    fprintf(item_base_file, "                <Float>%g</Float>\n", fValue);
                }
                fprintf(item_base_file, "            </DescriptionVars>\n");
            }
            fprintf(item_base_file, "        </Effect>\n");
        }

        fprintf(item_base_file, "    </ActivatableEffects>\n");
    }

    if (EquipableEffectCount > 0)
    {
        fprintf(item_base_file, "    <EquipableEffects unknown1=\"%d\" unknown2=\"%d\" unknown3=\"%d\" unknown4=\"%d\">\n",
            ntohl(EquipableUnknownData[0]),
            ntohl(EquipableUnknownData[1]),
            ntohl(EquipableUnknownData[2]),
            ntohl(EquipableUnknownData[3]));

        p = (unsigned char *) EquipableEffectData;
        for (i = 0; i < EquipableEffectCount; i++)
        {
            // skip the name
            string_length = *((short *) p);
            p += 2;
            char *name = (char *) p;
            int name_length = string_length;
            p += string_length; // skip the string

            // skip the description
            string_length = *((short *) p);
            p += 2;
            char *description = (char *) p;
            int description_length = string_length;
            p += string_length; // skip the string

            // skip the tooltip
            string_length = *((short *) p);
            p += 2;
            char *tooltip = (char *) p;
            int tooltip_length = string_length;
            p += string_length; // skip the string

            long EquipableLongNameVariableCount = ntohl(*((long *) p));
            p += 4;

            long EquipableDescriptionVariableCount = ntohl(*((long *)p));
            p += 4;

            long * EquipableDescriptionVariableData = (long *) p;

            // Skip the description variables
            for (j = 0; j < EquipableDescriptionVariableCount; j++)
            {
                p += 4;
            }

            long Flag1 = *((long *) p);
            p += 4;

            long Flag2 = *((long *) p);
            p += 4;

            fprintf(item_base_file, "        <Effect  flag1=\"%d\" flag2=\"%d\">\n", Flag1, Flag2);
            fprintf(item_base_file, "            <Name>%.*s</Name>\n", name_length, name);
            fprintf(item_base_file, "            <Description>%.*s</Description>\n", description_length, description);
            fprintf(item_base_file, "            <Tooltip>%.*s</Tooltip>\n", tooltip_length, tooltip);
            if (EquipableDescriptionVariableCount > 0)
            {
                fprintf(item_base_file, "            <DescriptionVars>\n");
                for (j = 0; j < EquipableDescriptionVariableCount; j++)
                {
                    // The float data is in network order
                    iValue = ntohl(EquipableDescriptionVariableData[j]);
                    fValue = *((float *) &iValue);
                    fprintf(item_base_file, "                <Float>%g</Float>\n", fValue);
                }
                fprintf(item_base_file, "            </DescriptionVars>\n");
            }
            fprintf(item_base_file, "        </Effect>\n");
        }

        fprintf(item_base_file, "    </EquipableEffects>\n");
    }


    fprintf(item_base_file, "</ItemBase>\n\n");
}

void CreateSectorContentXmlFiles()
{
    int i, j;
    FILE *f = fopen("SectorContent.xml", "w");
    if (f)
    {
        fprintf(f, "<!-- SectorContent.xml -->\n");
        fprintf(f, "<SectorContent>\n");
        for (int sector_index = 0; sector_index < g_NumSectors; sector_index++)
        {
            long sector_id = g_SectorList[sector_index];
            fprintf(f, "    <Sector ID=\"%d\">\n", sector_id);
            for (i = 0; i < g_SectorContentCount; i++)
            {
                if (g_SectorContentArray[i].SectorID == sector_id)
                {
                    SectorContent *p = &g_SectorContentArray[i];
                    Create *create = (Create *) (&p->CreatePacket[4]);
                    fprintf(f, "        <GameObject GameID=\"%d\" Scale=\"%g\" BaseAsset=\"%d\" Type=\"%d\"",
                        p->GameID, create->Scale, create->BaseAsset, create->Type);
                    if ((create->HSV[0] != 0.0) ||
                        (create->HSV[1] != 0.0) ||
                        (create->HSV[2] != 0.0))
                    {
                        fprintf(f, " HSV=\"%g,%g,%g\"",
                            create->HSV[0], create->HSV[1], create->HSV[2]);
                    }
                    fprintf(f, ">\n");

                    bool have_name = false;
                    unsigned char *data = NULL;
                    for (j = 0; j < 3; j++)
                    {
                        if (p->AuxDataPackets[j])
                        {
                            data = p->AuxDataPackets[j] + 10;
                            if ((data[0] == 0x01) && ((data[1] & 0x1f) == 0x12))
                            {
                                // This is a "name" AuxData packet
                                short name_length = *((short *) &data[2]);
                                char *name = (char *) &data[4];
                                if ((name_length > 0) && (isprint(name[0])))
                                {
                                    fprintf(f, "            <Name>%.*s</Name>\n", name_length, name);
                                    have_name = true;
                                }
                            }
                            else if ((data[0] == 0x01) && (data[1] == 0x16))
                            {
                                if (data[2] == 0x04)
                                {
                                    data++;
                                    // This is a "name" AuxData packet
                                    short name_length = *((short *) &data[2]);
                                    char *name = (char *) &data[4];
                                    if ((name_length > 0) && (isprint(name[0])))
                                    {
                                        fprintf(f, "            <Name>%.*s</Name>\n", name_length, name);
                                        have_name = true;
                                    }
                                    else
                                    {
                                        data += 13;
                                        // This is a "name" AuxData packet
                                        short name_length = *((short *) &data[2]);
                                        char *name = (char *) &data[4];
                                        if ((name_length > 0) && (isprint(name[0])))
                                        {
                                            fprintf(f, "            <Name>%.*s</Name>\n", name_length, name);
                                            have_name = true;
                                        }
                                    }
                                }
                                else if ((data[2] == 0xe0) || (data[2] == 0xec))
                                {
                                    data += 2;
                                    // This is a "name" AuxData packet
                                    short name_length = *((short *) &data[2]);
                                    char *name = (char *) &data[4];
                                    if ((name_length > 0) && (isprint(name[0])))
                                    {
                                        fprintf(f, "            <Name>%.*s</Name>\n", name_length, name);
                                        have_name = true;
                                    }
                                }
                                else
                                {
                                    data += 14;
                                    // This is a "name" AuxData packet
                                    short name_length = *((short *) &data[2]);
                                    char *name = (char *) &data[4];
                                    if ((name_length > 0) && (isprint(name[0])))
                                    {
                                        fprintf(f, "            <Name>%.*s</Name>\n", name_length, name);
                                        have_name = true;
                                    }
                                }
                            }
                            else if ((data[0] == 0x01) && (data[1] == 0xF6) && (data[2] == 0x71))
                            {
                                short name_length = *((short *) &data[7]);
                                char *name = (char *) &data[9];
                                if ((name_length > 0) && (isprint(name[0])))
                                {
                                    fprintf(f, "            <Name>%.*s</Name>\n", name_length, name);
                                    have_name = true;
                                }
                            }
                        }
                    }

                    short basset = *((short *) &p->CreatePacket[12]);
                    // planetary rings don't need a name

                    if (!have_name && p->AuxDataPackets[0] && (basset != 1018))
                    {
                        data = p->AuxDataPackets[0];
                        printf("No name!\n");
                    }

                    for (j = 0; j < 2; j++)
                    {
                        if (p->ActivateRenderStatePackets[j])
                        {
                            long id = *((long *) (p->ActivateRenderStatePackets[j] + 8));
                            fprintf(f, "            <ActivateRenderState RenderStateID=\"%d\"/>\n", id);
                        }
                    }

                    for (j = 0; j < 3; j++)
                    {
                        if (p->ObjectEffectPackets[j])
                        {
                            char Bitmask = *((char *) (p->ObjectEffectPackets[j] + 4));
                            short EffectDescID = *((short *) (p->ObjectEffectPackets[j] + 9));
                            fprintf(f, "            <ObjectEffect Bitmask=\"%d\" EffectDescID=\"%d\"",
                                Bitmask, EffectDescID);
                            unsigned char *data = p->ObjectEffectPackets[j] + 11;
                            if (Bitmask & 0x01)
                            {
                                long EffectID = *((long *) data);
                                fprintf(f, " EffectID=\"%d\"", EffectID);
                                data += 4;
                            }
                            if (Bitmask & 0x02)
                            {
                                long TimeStamp = *((long *) data);
                                fprintf(f, " TimeStamp=\"%d\"", TimeStamp);
                                data += 4;
                            }
                            if (Bitmask & 0x04)
                            {
                                short Duration = *((short *) data);
                                fprintf(f, " Duration=\"%d\"", Duration);
                                data += 2;
                            }
                            if (Bitmask & 0x08)
                            {
                                float Scale = *((float *) data);
                                fprintf(f, " Scale=\"%g\"", Scale);
                                data += 4;
                            }
                            if (Bitmask & 0x70)
                            {
                                float h = 0;
                                float s = 0;
                                float v = 0;
                                if (Bitmask & 0x10)
                                {
                                    h = *((float *) data); data += 4;
                                }
                                if (Bitmask & 0x20)
                                {
                                    s = *((float *) data); data += 4;
                                }
                                if (Bitmask & 0x40)
                                {
                                    v = *((float *) data); data += 4;
                                }
                                fprintf(f, " HSVShift=\"%g,%g,%g\"", h, s, v);
                            }
                            fprintf(f, "/>\n");
                        }
                    }
                    if (p->RelationshipPacket)
                    {
                        long Reaction = *((long *) (p->RelationshipPacket + 8));
                        char IsAttacking = *((char *) (p->RelationshipPacket + 12));
                        fprintf(f, "            <Relationship Reaction=\"%d\" IsAttacking=\"%d\"/>\n",
                            Reaction, IsAttacking);
                    }
                    if (p->SimplePositionalUpdate)
                    {
                        long TimeStamp = *((long *) (p->SimplePositionalUpdate + 8));
                        float *data = (float *) (p->SimplePositionalUpdate + 12);
                        fprintf(f, "            <SimplePositionalUpdate TimeStamp=\"%d\" Position=\"%g,%g,%g\" Orientation=\"%g,%g,%g,%g\" Velocity=\"%g,%g,%g\"/>\n",
                            TimeStamp, data[0], data[1], data[2],
                            data[3], data[4], data[5], data[6],
                            data[7], data[8], data[9]);
                    }
                    if (p->AdvancedPositionalUpdate)
                    {
                        short Bitmask = *((short *) (p->AdvancedPositionalUpdate + 4));
                        long TimeStamp = *((long *) (p->AdvancedPositionalUpdate + 10));
                        float *fData = (float *) (p->AdvancedPositionalUpdate + 14);
                        long *iData = (long *) (p->AdvancedPositionalUpdate + 14);

                        fprintf(f, "            <AdvancedPositionalUpdate Bitmask=\"%d\" TimeStamp=\"%d\" Position=\"%g,%g,%g\" Orientation=\"%g,%g,%g,%g\" MovementID=\"%d\"",
                            Bitmask, TimeStamp, 
                            fData[0], fData[1], fData[2],
                            fData[3], fData[4], fData[5], fData[6],
                            iData[7]);

                        int index = 8;
                        if (Bitmask & 0x0001)
                        {
                            fprintf(f, " CurrentSpeed=\"%g\"", fData[index++]);
                        }
                        if (Bitmask & 0x0002)
                        {
                            fprintf(f, " SetSpeed=\"%g\"", fData[index++]);
                        }
                        if (Bitmask & 0x0004)
                        {
                            fprintf(f, " Acceleration=\"%g\"", fData[index++]);
                        }
                        if (Bitmask & 0x0008)
                        {
                            fprintf(f, " RotY=\"%g\"", fData[index++]);
                        }
                        if (Bitmask & 0x0010)
                        {
                            fprintf(f, " DesiredY=\"%g\"", fData[index++]);
                        }
                        if (Bitmask & 0x0020)
                        {
                            fprintf(f, " RotZ=\"%g\"", fData[index++]);
                        }
                        if (Bitmask & 0x0040)
                        {
                            fprintf(f, " DesiredZ=\"%g\"", fData[index++]);
                        }
                        if (Bitmask & 0x0080)
                        {
                            fprintf(f, " ImpartedVelocity=\"%g,%g,%g\"",
                                fData[index], fData[index+1], fData[index+2]);
                            fprintf(f, " ImpartedSpin=\"%g\"", fData[index+3]);
                            fprintf(f, " ImpartedRoll=\"%g\"", fData[index+4]);
                            fprintf(f, " ImpartedPitch=\"%g\"", fData[index+5]);
                            index += 6;
                        }
                        if (Bitmask & 0x0100)
                        {
                            fprintf(f, " UpdatePeriod=\"%d\"", iData[index++]);
                        }
                        fprintf(f, "/>\n");
                    }
                    if (p->PlanetPositionalUpdate)
                    {
                        long TimeStamp = *((long *) (p->PlanetPositionalUpdate + 8));
                        float *fData = (float *) (p->PlanetPositionalUpdate + 12);
                        long *iData = (long *) (p->PlanetPositionalUpdate + 12);
                        fprintf(f, "            <PlanetPositionalUpdate TimeStamp=\"%d\" Position=\"%g,%g,%g\" OrbitID=\"%d\" OrbitDist=\"%g\" OrbitAngle=\"%g\" OrbitRate=\"%g\" RotateAngle=\"%g\" RotateRate=\"%g\" TiltAngle=\"%g\"/>\n",
                            TimeStamp,
                            fData[0], fData[1], fData[2],
                            iData[3],
                            fData[4], fData[5], fData[6],
                            fData[7], fData[8], fData[9]);
                    }
                    if (p->ConstantPositionalUpdate)
                    {
                        float *data = (float *) (p->ConstantPositionalUpdate + 8);
                        fprintf(f, "            <ConstantPositionalUpdate Position=\"%g,%g,%g\" Orientation=\"%g,%g,%g,%g\"/>\n",
                            data[0], data[1], data[2],
                            data[3], data[4], data[5], data[6]);
                    }
                    if (p->ComponentPositionalUpdate)
                    {
                        long TimeStamp = *((long *) (p->ComponentPositionalUpdate + 8));
                        float *fData = (float *) (p->ComponentPositionalUpdate + 12);
                        long *iData = (long *) (p->ComponentPositionalUpdate + 12);
                        fprintf(f, "            <ComponentPositionalUpdate TimeStamp=\"%d\" Position=\"%g,%g,%g\" Orientation=\"%g,%g,%g,%g\" Velocity=\"%g,%g,%g\" ImpartedDecay=\"%g\" TractorSpeed=\"%g\" TractorID=\"%d\" TractorEffectID=\"%d\"/>\n",
                            TimeStamp,
                            fData[0], fData[1], fData[2],
                            fData[3], fData[4], fData[5], fData[6],
                            fData[7], fData[8], fData[9],
                            fData[10], fData[11],
                            iData[12], iData[13]);
                    }

                    // TODO: Decode the AuxData packet!

                    if (p->NavigationPacket)
                    {
                        float Signature = *((float *) (p->NavigationPacket + 8));
                        char PlayerHasVisited = p->NavigationPacket[12];
                        long NavType = *((long *) (p->NavigationPacket + 13));
                        char IsHuge = p->NavigationPacket[17];

                        fprintf(f, "            <Navigation Signature=\"%.0f\" PlayerHasVisited=\"%d\" NavType=\"%d\" IsHuge=\"%d\"/>\n",
                            Signature, PlayerHasVisited, NavType, IsHuge);
                    }

                    fprintf(f, "        </GameObject>\n");
                }
            }
            fprintf(f, "    </Sector>\n");
        }

        fprintf(f, "</SectorContent>\n");
        fclose(f);
    }
}

