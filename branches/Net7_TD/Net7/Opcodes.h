// Opcodes.h

#ifndef _OPCODES_H_INCLUDED_
#define _OPCODES_H_INCLUDED_

#define ENB_OPCODE_0000_VERSION_REQUEST                 0x0000
#define ENB_OPCODE_0001_VERSION_RESPONSE                0x0001
#define ENB_OPCODE_0002_LOGIN                           0x0002
#define ENB_OPCODE_0004_CREATE                          0x0004
#define ENB_OPCODE_0005_START                           0x0005
#define ENB_OPCODE_0006_START_ACK                       0x0006
#define ENB_OPCODE_0007_REMOVE                          0x0007
#define ENB_OPCODE_0008_SIMPLE_POSITIONAL_UDPATE        0x0008
#define ENB_OPCODE_0009_OBJECT_EFFECT                   0x0009
#define ENB_OPCODE_0010_DECAL                           0x0010
#define ENB_OPCODE_0011_COLORIZATION                    0x0011
#define ENB_OPCODE_0012_TURN                            0x0012	// Added by David
#define ENB_OPCODE_0013_TILT                            0x0013  // 
#define ENB_OPCODE_0014_MOVE                            0x0014  //
#define ENB_OPCODE_0015_REMOVE_EFFECT                   0x000f  // 
#define ENB_OPCODE_009B_WARP							0x009B  // Warp TODO
#define ENB_OPCODE_0027_INVENTORYMOVE					0x0027  // Warp TODO
#define ENB_OPCODE_0017_REQUEST_TARGET                  0x0017
#define ENB_OPCODE_0019_SET_TARGET                      0x0019
#define ENB_OPCODE_001A_DEBUG                           0x001a
#define ENB_OPCODE_001B_AUX_DATA                        0x001b
#define ENB_OPCODE_001C_PLAYER_VAR_AUX_DATA             0x001c
#define ENB_OPCODE_001D_MESSAGE_STRING                  0x001d
#define ENB_OPCODE_001F_TRADE			                0x001f	// David
#define ENB_OPCODE_0025_ITEM_BASE                       0x0025
#define ENB_OPCODE_002C_ACTION                          0x002c
#define ENB_OPCODE_002A_SET_ZBAND                       0x002a
#define ENB_OPCODE_002B_SET_BBOX                        0x002b
#define ENB_OPCODE_002E_OPTION                          0x002e
#define ENB_OPCODE_002F_INIT_RENDER_STATE               0x002f
#define ENB_OPCODE_0030_ACTIVATE_RENDER_STATE           0x0030
#define ENB_OPCODE_0031_ACTIVATE_NEXT_RENDER_STATE		0x0031	
#define	ENB_OPCODE_0032_DEACTIVATE_RENDER_STATE         0x0032	
#define ENB_OPCODE_0033_CLIENT_CHAT                     0x0033
#define ENB_OPCODE_0034_CLIENT_SET_TIME                 0x0034
#define ENB_OPCODE_0035_MASTER_JOIN                     0x0035
#define ENB_OPCODE_0036_SERVER_REDIRECT                 0x0036
#define ENB_OPCODE_0037_CLIENT_AVATAR                   0x0037
#define ENB_OPCODE_003A_SERVER_HANDOFF                  0x003a
#define ENB_OPCODE_003C_CLIENT_TYPE                     0x003c
#define ENB_OPCODE_003E_ADVANCED_POSITIONAL_UPDATE      0x003e
#define ENB_OPCODE_003F_PLANET_POSITIONAL_UPDATE        0x003f
#define ENB_OPCODE_0040_CONSTANT_POSITIONAL_UPDATE      0x0040
#define ENB_OPCODE_0042_SERVER_PARAMETERS               0x0042
#define ENB_OPCODE_0044_REQUEST_TIME                    0x0044
#define ENB_OPCODE_0046_COMPONENT_POSITIONAL_UPDATE     0x0046
#define ENB_OPCODE_0047_CLIENT_SHIP                     0x0047
#define ENB_OPCODE_004A_CREATE_ATTACHMENT               0x004a
#define ENB_OPCODE_004E_STARBASE_REQUEST                0x004e
#define ENB_OPCODE_004F_STARBASE_SET                    0x004f
#define ENB_OPCODE_0052_LOUNGE_NPC                      0x0052
#define ENB_OPCODE_0054_TALK_TREE                       0x0054
#define ENB_OPCODE_0055_SELECT_TALK_TREE                0x0055
#define ENB_OPCODE_0056_TALK_TREE_ACTION                0x0056
#define ENB_OPCODE_0057_SKILL_UP		                0x0057	// Added: David
#define ENB_OPCODE_0058_SKILL_ABILITY                   0x0058
#define ENB_OPCODE_005A_VERB_REQUEST                    0x005a
#define ENB_OPCODE_005C_VERB_UPDATE                     0x005c
#define ENB_OPCODE_0061_AVATAR_DESCRIPTION              0x0061
#define ENB_OPCODE_006A_CLIENT_SOUND                    0x006a
#define ENB_OPCODE_006D_GLOBAL_CONNECT                  0x006d
#define ENB_OPCODE_006E_GLOBAL_TICKET_REQUEST           0x006e
#define ENB_OPCODE_006F_GLOBAL_TICKET                   0x006f
#define ENB_OPCODE_0070_GLOBAL_AVATAR_LIST              0x0070
#define ENB_OPCODE_0071_GLOBAL_DELETE_CHARACTER         0x0071
#define ENB_OPCODE_0072_GLOBAL_CREATE_CHARACTER         0x0072
#define ENB_OPCODE_0075_GLOBAL_ERROR					0x0075  // Added: David
#define ENB_OPCODE_007F_MANUFACTURE_SET_MANUFACTURE_ID  0x007f
#define ENB_OPCODE_0088_PETITION_STUCK                  0x0088
#define ENB_OPCODE_0089_RELATIONSHIP                    0x0089
#define ENB_OPCODE_0092_CAMERA_CONTROL                  0x0092
#define ENB_OPCODE_0097_GALAXY_MAP                      0x0097
#define ENB_OPCODE_0098_GALAXY_MAP_REQUEST              0x0098
#define ENB_OPCODE_0099_NAVIGATION                      0x0099
#define ENB_OPCODE_009D_STARBASE_AVATAR_CHANGE          0x009D  // Client to Server
#define ENB_OPCODE_009E_STARBASE_AVATAR_CHANGE          0x009E  // Server to Client
#define ENB_OPCODE_009F_STARBASE_ROOM_CHANGE            0x009f
#define ENB_OPCODE_00A0_STARBASE_ROOM_CHANGE			0x00A0	// Server to Client
#define ENB_OPCODE_00A1_TRIGGER_EMOTE                   0x00a1
#define ENB_OPCODE_00A2_NOTIFY_EMOTE                    0x00a2
#define ENB_OPCODE_00A3_CLIENT_CHAT_REQUEST             0x00a3
#define ENB_OPCODE_00B2_NAME_DECAL                      0x00b2
#define ENB_OPCODE_00B4_SUBPARTS                        0x00b4
#define ENB_OPCODE_00B9_LOGOFF_REQUEST                  0x00b9
#define ENB_OPCODE_00BA_LOGOFF_CONFIRMATION             0x00ba

#define ENB_OPCODE_7801_SECTOR_ASSIGNMENT               0x7801
#define ENB_OPCODE_7802_REQUEST_CHARACTER_DATA          0x7802
#define ENB_OPCODE_7902_CHARACTER_DATA                  0x7902
#define ENB_OPCODE_7803_SECTOR_SHUTDOWN                 0x7803
#define ENB_OPCODE_7804_CHAT_MESSAGE                    0x7804
#define ENB_OPCODE_7805_WHERE_IS_PLAYER                 0x7805
#define ENB_OPCODE_7905_PLAYER_LOCATION                 0x7905


/*
State  Decimal  Hexadecimal  Description of the Opcode  class offset  
Working 0  00  VersionRequest  0xB0C2D0  
Working 1  01  VersionResponse  0xB0C300  
Partially 2  02  Login  0xB0B014  
Partially 3  03  Logoff  0xB0B044  
 4  04  Create  0xB0A140  
Working 5  05  Start  0xB0BC00  
Working 6  06  StartAck  0xB0BC30  
Working 7  07  Remove  0xB0B6E8  
 8  08  Simple_Positional_Update  0xB0C0E8  
 9  09  Object_Effect  0xB0A3A0  
Partially 10  0A  Point_Effect  0xB0A408  
Untested 11  0B  Object_To_Object_Effect  0xB0A438  
Untested 12  0C  Object_To_Effect_Effect  0xB0A46C  
Untested 13  0D  Object_To_Point_Effect  0xB0A49C  
Untested 14  0E  Object_To_Object_Duration_Linked_Effect  0xB0A4CC  
Untested 15  0F  Remove_Effect  0xB0A4FC  
Working 16  10  Decal  0xB0A200  
 17  11  Colorization  0xB0A110  
Working 18  12  Turn  0xB0C048  
Working 19  13  Tilt  0xB0BF88  
Working 20  14  Move  0xB0B344  
Untested 21  15  Fire  0xB0A61C  
Untested 22  16  Fire Equipped  0xB0A61C  
Partially 23  17  Request_Target  0xB0BCF0  
Untested 24  18  Request_Target_Target  0xB0BD20  
Partially 25  19  Set_Target  0xB0BD50  
Partially 26  1A  Debug  0xB0A1D0  
 27  1B  Aux_Data  0xB09CA4  
 28  1C  PlayerVar_Aux_Data  0xB09CE0  
Untested 29  1D  Message_String  0xB0B444  
Untested 30  1E  Group_Action  0xB0A734  
Untested 31  1F  Trade_Action  0xB0C018  
Untested 32  20  PriorityMessageLine  0xB09760  
Untested 33  21  QueueMessageLine  0xB09724  
Untested 34  22  PushMessageLine  0xB096E8  
Untested 35  23  SuperLine  0xB0B410  
Untested 36  24  ClearMessageLine  0xB0B3E0  
Untested 37  25  ItemBase  0xAEDE88  
Untested 38  26  Change_Base_Asset  0xB0A050  
Untested 39  27  Inventory  0xB0AE34  
Untested 40  28  InventorySort  0xB0AE64  
Untested 41  29  ItemStates  0xB0AEC4  
Partially 42  2A  Set_ZBand  0xB0B9B8  
Partially 43  2B  Set_BBox  0xB0B9E8  
Untested 44  2C  Action  0xB09C14  
Untested 45  2D  Action2  0xB09C44  
 46  2E  Option  0xB0B564  
 47  2F  Initialize_Render_State  0xB0B718  
 48  30  Activate_Render_State  0xB0B748  
 49  31  Activate_Next_Render_State_Once  0xB0B778  
 50  32  Deactivate_Render_State  0xB0B7A8  
 51  33  Client_Chat  0xB09E10  
 52  34  Set_Client_Time  0xB0BFE8  
 53  35  Master_Join  0xB0AFB4  
 54  36  Client_Redirect  0xB0B6B4  
 55  37  Client_Avatar  0xB09D1C  
 56  38  Spline  0xB0BB3C  
 57  39  NOT USED  
 58  3A  Server_Handoff  0xB0ADD4  
 59  3B  SectorServer_Ready  0xB0BB9C  
 60  3C  Set_Client_Type  0xB0A170  
 61  3D  Server_Stop  0xB0BBD0  
Partially 62  3E  Advanced_Positional_Update  0xB0C11C  
Partially 63  3F  Planet_Positional_Update  0xB0C16C  
Working 64  40  0x40 Constant_Positional_Update  0xB0C0B4  
Untested 65  41  FormationPositionalUpdate  0xB0C1D4  
 66  42  Server_Parameters  0xB0BB6C  
 67  43  Request_Transform_Change  0xB0B808  
Partially 68  44  Request_Time  0xB0BFB8  
 69  45  Request_Version  0xB0B838  
Untested 70  46  Component_Positional_Update  0xB0C1A0  
 71  47  Client_Ship  0xB0BA18  
 72  48  CreateCharacter  0xB08D64  
 73  49  DeleteCharacter  0xB08D98  
 74  4A  0x4A CreateAttachment  0xB09C74  
 75  4B  ModeRequest  0xB0B314  
 76  4C  ModeRequest  0xB0B2E4  
 77  4D  RegistrationResult  0xB08DC8  
 78  4E  Starbase_Request  0xB0B868  
 79  4F  Starbase_Set  0xB0B898  
 80  50  Starbase_Tow  0xB0B8C8  
 81  51  Request_Skill_Strings  0xB0B7D8  
 82  52  LoungeNPC  0xB0AFE4  
 83  53  NumPlayers?  0xB0A58C  
 84  54  TalkTree  0xB0BC60  
 85  55  Select_Talk_Tree  0xB0BCC0  
 86  56  TalkTreeAction  0xB0BC90  
 87  57  SkillAction  0xB0BA48  
 88  58  SkillAbility  0xB0BA78  
 89  59  HullUpgrade  0xB0AE04  
 90  5A  VerbRequest  0xB0C234  
 91  5B  VerbList  0xB0C268  
 92  5C  VerbUpdate  0xB0C29C  
 93  5D  UseInventoryItem  0xB0C204  
 94  5E  Chatstream  0xB0A020  
Untested 95  5F  Message_String  0xB0B374  
 96  60  Message_Time  0xB0B474  
 97  61  0x61 AvatarDescription  0xB09D4C  
 98  62  Dialog  0xB0A260  
 99  63  Broadcast  0xB09D7C  
 100  64  ClientDamage  0xB0A1A0  
 101  65  UITrigger  0xB0C078  
 102  66  Open_Interface  0xB0B534  
 103  67  GadgetControl  0xB0A65C  
 104  68  GadgetControl  0xB0A65C  
 105  69  ClickIndicator  0xB0A080  
 106  6A  Client_Sound  0xB0BAA8  
 107  6B  Client_Sound_Command  0xB0BAD8  
 108  6C  Note ?  0xB0B504  
 109  6D  GlobalConnect  0xB09398  
 110  6E  GlobalTicketRequest  0xB096B8  
 111  6F  GlobalTicket  0xB09688  
 112  70  GlobalAvatarList  0xB09368  
 113  71  GlobalDeleteCharacter  0xB09460  
 114  72  0x72 GlobalCreateCharacter  0xB09430  
 115  73  GlobalHello  0xB094C0  
 116  74  GlobalPlayerStatus  0xB095F4  
 117  75  GlobalError  0xB09490  
 118  76  GlobalLogin  0xB095C4  
 119  77  GlobalSetReferrer  0xB04170  
 120  78  GlobalSetReferrerResponse  0xB09628  
 121  79  ManufactureItemCategoryRequest  0xB0B104  
 122  7A  ManufactureItemCategoryRequest  0xB0B134  
 123  7B  ManufactureSetItemID  0xB0B164  
 124  7C  RefinerySetItemID  0xB0B194  
 125  7D  ManufactureSetSlotID  0xB0B1C4  
 126  7E  ManufactureAction  0xB0B1F4  
 127  7F  ManufactureSetManufactureID  0xB0B224  
 128  80  ManufactureTechLevelFilterRequest  0xB0B254  
 129  81  RecustomizeShipStart  0xB0B5C4  
 130  82  RecustomizeShipDone  0xB0B5F4  
 131  83  RecustomizeAvatarStart  0xB0B624  
 132  84  RecustomizeAvatarDone  0xB0B654  
 133  85  RecustomizeAvatarUpdate  0xB0B684  
 134  86  MissionForfeitRequest  0xB0B284  
 135  87  MissionDismissRequest  0xB0B2B4  
 136  88  PetitionStuck  0xB0B594  
 137  89  Relationship  0xB09B14  
 138  8A  Player_Object_Reaction_Update  0xB09B44  
 139  8B  AttackerUpdates  0xB09B94  
 140  8C  LootHulkPermission  0xB09AE4  
 141  8D  IncapacitanceDetailsRequest  0xB099F4  
 142  8E  IncapacitanceDistressBeacon  0xB09A24  
 143  8F  Request_Rescue  0xB09A54  
 144  90  IncapacitanceDetails  0xB09A84  
 145  91  IncapacitanceRescueConfirmation  0xB09AB4  
 146  92  CameraControl  0xB09DDC  
 147  93  JobList  0xB0AEF4  
 148  94  JobDescription  0xB0AF24  
 149  95  JobDelete  0xB0AF54  
 150  96  JobAcceptReply  0xB0AF84  
 151  97  GalaxyMap2  0xB0992C  
 152  98  GalaxyMap2  0xB0992C  
 153  99  0x99 Navigation  0xB0B4A4  
 154  9A  NavDelete  0xB0B4D4  
 155  9B  Warp  0xB0C330  
 156  9C  Warp_Index  0xB0C360  
 157  9D  Starbase_Avatar_Change2  0xB0B8F8  
 158  9E  Starbase_Avatar_Change  0xB0B928  
 159  9F  Starbase_Room_Change  0xB0B958  
 160  A0  Starbase_Room_Update  0xB0B988  
Working 161  A1  0xA1 TriggerEmote  0xB0A52C  
Partially 162  A2  NotifyEmote  0xB0A55C  
 163  A3  ClientChatRequest  0xB09E40  
 164  A4  ClientChatList  0xB09EA0  
 165  A5  ClientChatEvent  0xB09E70  
 166  A6  ClientChatError  0xB09ED0  
 167  A7  ChatBasicRequest  0xB09F00  
 168  A8  ChatProxyLogin  0xB09F30  
 169  A9  ChatProxyRequest  0xB09F60  
 170  AA  ChatProxyResponse  0xB09F90  
 171  AB  ChatProxyResponseList  0xB09FC0  
 172  AC  ChatProxyEvent  0xB09FF0  
 173  AD  GroupServerLogin  0xB0A764  
 174  AE  GroupServerRequest  0xB0A7C4  
 175  AF  GroupServerResponse  0xB0A7F4  
 176  B0  GroupServerCreate  0xB0A824  
 177  B1  GroupServerUpdate  0xB0A854  
 178  B2  NameDecal  0xB0A230  
 179  B3  ShipNameValidate  0xB09658  
 180  B4  SubParts  0xB09BE4  
 181  B5  FindPlayerRequest  0xB0A5BC  
 182  B6  FindPlayerResponse  0xB0A5EC  
 183  B7  NoteList  0xB0A0B0  
 184  B8  NoteContent  0xB0A0E0  
Partially 185  B9  LogoffRequest  0xB0B074  
Untested 186  BA  LogoffConfirmation  0xB0B0A4  
Untested 187  BB  LogoffConfirmation2  0xB0B0D4  
 188  BC  CTA_Request  0xB097DC  
 189  BD  CTA_Response  0xB0980C  
 190  BE  ConfirmedActionOffer  0xB0989C  
 191  BF  ConfirmedActionTimeout  0xB098CC  
 192  C0  ConfirmedActionResponse  0xB098FC  
 193  C1  GuildPlayerOnline  0xB0AA64  
 194  C2  GuildAdjustContrib  0xB0AAF4  
 195  C3  GuildStatusUpdate  0xB0AC14  
 196  C4  GuildBeginCreation  0xB0AB24  
 197  C5  GuildLeaderAcceptClient  0xB0A8E4  
 198  C6  GuildLeaderAcceptSector  0xB0AB54  
 199  C7  GuildRecruitConfirmGuild  0xB0AB84  
 200  C8  GuildRecruitConfirmSector  0xB0A914  
 201  C9  GuildRecruitAcceptClient  0xB0A944  
 202  CA  GuildRecruitAcceptSector  0xB0ABB4  
 203  CB  GuildSimpleGuildSector  0xB0AA94  
 204  CC  GuildSimpleSectorClient  0xB0A884  
 205  CD  GuildSimpleClientSector  0xB0A8B4  
 206  CE  GuildSimpleSectorGuild  0xB0AAC4  
 207  CF  GuildMessageGuild  0xB0ABE4  
 208  D0  GuildMessageSector  0xB0A974  
 209  D1  GuildPermissions  0xB0AC44  
 210  D2  GuildPlayerPermissions  0xB0A9A4  
 211  D3  GuildRankNamesSector  0xB0AA04  
 212  D4  GuildRankNamesRequestClient  0xB0A9D4  
 213  D5  GuildRankNamesGuild  0xB0ACA4  
 214  D6  GuildRankNamesRequestSector  0xB0AC74  
 215  D7  GuildGMRenameConfirmGuild  0xB0ACD4  
 216  D8  GuildGMRenameConfirmSector  0xB0AA34  
 217  D9  GuildGMRenameAcceptSector  0xB0AD04  
 218  DA  GuildDeletePlayer  0xB0AD34  
 219  DB  GuildPlayerInfoDeleted  0xB0AD64  
 220  DC  GuildDeductFee  0xB0AD94  
 221  DD  GPSRequest  0xB0A6D4  
 222  DE  GPSResponse  0xB0A704  
 223  DF  DirLockRequest  0xB0A2C4  
 224  E0  DirLockResponseGlobal  0xB0A32C  
 225  E1  DirLockResponseSector  0xB0A2F8  
 226  E2  DirLockRelease  0xB0A360  
 227  E3  GlobalLockRequest  0xB09528  
 228  E4  GlobalLockResponse  0xB0955C  
 229  E5  GlobalLockRelease  0xB09590  
 230  E6  GlobalContainerRequest  0xB093C8  
 231  E7  GlobalContainerResponse  0xB093FC  
 232  E8  SpendReferralPoint  0xB0BB08  
 233  E9  ReferralPointsAvailable  0xB09DAC  
 234  EA  GM_Edit_Player_Inventory  0xB0983C  
 235  EB  GM_View_Mission_Log  0xB0986C  
 236  EC  GM_Destroy_Item  0xB09994  
 237  ED  GM_Reset_Mission  0xB099C4  

 5000  1388  Task_Hello  0xB0BD80  
 5001  1389  Sector_Hello  0xB0BDB4  
 5002  138A  Task_Start  0xB0BDE8  
 5003  138B  Task_Stop  0xB0BE1C  
 5004  138C  Task_Request  0xB0BE50  
 5005  138D  Task_Ping  0xB0BE84  
 5006  138E  Task_Pong  0xB0BEB8  
 5007  138F  NOT_USED  
 5008  1390  NOT_USED  
 5009  1391  PlayerStatus  0xB0BEEC  
 5010  1392  SectorList  0xB0BF20  
 5011  1393  RequestSectorList  0xB0BF54  

 30000  7530  GameEditorLogin  0xB08F78  
 30001  7531  GameEditorLogout  0xB08FA8  
 30002  7532  GameEditorRequestKeyValues  0xB08EB8  
 30003  7533  GameEditorCancelKeyValues  0xB08EE8  
 30004  7534  GameEditorModifyKeyValues  0xB08F48  
 30005  7535  GameEditorDelete  0xB09098  
 30006  7536  GameEditorMove  0xB090C8  
 30007  7537  GameEditorRequestTemplateList  0xB09128  
 30008  7538  GameEditorCreateEmpty  0xB09038  
 30009  7539  GameEditorCreateCancel  0xB09068  
 30010  753A  GameEditorCreateFinal  0xB09008  
 30011  753B  GameEditorRequestSpawnDefaults  0xB09308  
 30012  753C  GameEditorClone  0xB08E58  
 30013  753D  GameEditorRequestPlanetUnitTypes  0xB09188  
 30014  753E  GameEditorRequestPlanetUnitDetails  0xB091E8  
 30015  753F  GameEditorSaveSector  0xB092A8  
*/

#endif // _OPCODES_H_INCLUDED_

