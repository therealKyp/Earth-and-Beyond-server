// PacketStructures.h

#ifndef _PACKET_STRUCTURES_H_INCLUDED_
#define _PACKET_STRUCTURES_H_INCLUDED_

struct EnbTcpHeader
{
    short   size;
    short   opcode;
} ATTRIB_PACKED;

struct VersionRequest
{
    long    Major;      // 4 bytes
    long    Minor;      // 4 bytes
} ATTRIB_PACKED;

//New header for use with UDP comms.
//If we have the player_id it makes things a lot simpler
struct EnbUdpHeader
{
    short size;
    short opcode;
    long player_id;
    long packet_sequence;
};

/*

  Personality:
    0 = Calm Male Personality           MacGregor, Herra, Cassel, Silva, Loric, Grayfeather, P3889
    1 = Reserved Female Personality     Kathrada, Ariad, Vinda
    2 = Macho Male Personality          Kahn, ShouTzu, Var
    3 = Wild Male Personality           ?
    4 = Sexy Female Personality         deWinter
    5 = Tomboy Female Personality       Amah
    6 = Undefined / Placeholder         -
    7 = Megan Female Personality        Megan
    8 = Male Cockpit Personality        -
    9 = Female Cockpit Personality      -
    10 = Vrix personality               V'rix

  Head and Body:
    0  = Megan (F,7)
    1  = MacGregor (M,0)
    2  = Morgan Thorne (F,5?)
    3  = Lady deWinter (F,4)
    4  = Kahn (M,2)
    5  = Merjan Kathrada (F,1)
    6  = Nostradamus Smythe (M,?)
    7  = Shou Tzu (M,2)
    8  = Herra (M,0)
    9  = Var (M,2)
    10 = Ariad (F,1)
    11 = Cassel (M,0)
    12 = Vinda (F,1)
    13 = Kayen Silva (M,0)
    14 = Loric deGrey (M,0)
    15 = Amah (F,5)
    17 = Grayfeather (M,0)
    19 = P3889 (M,0)
    100 = V'rix (?,10)

*/

struct AvatarData
{
    char    avatar_first_name[20];      // 14   d4  20
    char    avatar_last_name[20];       // 28   e8  20
    long    avatar_type;                // 04   08  4
    char    filler1;                    //      0c  -
    char    avatar_version;             // 09   0d  1
                                        //      0e
                                        //      0f
    long    race;                       // 0c   10  4
    long    profession;                 // 10   14  4
    long    gender;                     // 14   18  4
    long    mood_type;                  // 18   1c  4

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

    long    shirt_primary_metal;        // 98   9c  4
    long    shirt_secondary_metal;      // 9c   a0  4
    long    pants_primary_metal;        // a0   a4  4
    long    pants_secondary_metal;      // a4   a8  4

    char    filler2;                    //          1?

    float   height_weight_1[5];         //      ac  20
    float   height_weight_2[5];         //      c0  20
} ATTRIB_PACKED;  // 241 bytes

struct AvatarInfo
{
    // NOTE: All fields are in Big Endian format -- use ntohl to convert!
    long    avatar_slot;        // 0 to 4 = 0
    long    sector_id;          // 1071
    long    galaxy_id;          // 1
    long    count;              // 5
    long    avatar_id_msb;      // 0
    long    avatar_id_lsb;      // 1
    long    account_id_msb;     // 0
    long    account_id_lsb;     // 2
    long    admin_level;        // 0
    long    gm_flag;            // 1
    long    combat_level;       // 0
    long    explore_level;      // 0
    long    trade_level;        // 0
    char    location[81];       // "Saturn"
} ATTRIB_PACKED;  // 133 bytes

struct ColorInfo
{
    float   HSV[3];
    char    flat;
    long    metal;
} ATTRIB_PACKED;  // 17 bytes

struct ShipData
{
    long    race;                           // 00
    long    profession;                     // 04
    long    hull;                           // 0c
    long    wing;                           // 08
    long    decal;                          // 10

    char    ship_name[26];                  // 14
    float   ship_name_color[3];             // 2e

    ColorInfo   HullPrimaryColor;           // 3a, 46, 47
    ColorInfo   HullSecondaryColor;         // 4b, 57, 58
    ColorInfo   ProfessionPrimaryColor;     // 5c, 68, 69
    ColorInfo   ProfessionSecondaryColor;   // 6d, 79, 7a
    ColorInfo   WingPrimaryColor;           // 7e, 8a, 8b
    ColorInfo   WingSecondaryColor;         // 8f, 9b, 9c
    ColorInfo   EnginePrimaryColor;         // a0, ac, ad
    ColorInfo   EngineSecondaryColor;       // b1, bd, be
} ATTRIB_PACKED;  // 194 bytes

struct Galaxy
{
    char    Name[64];
    long    GalaxyID;
    long    IP_Address;
    short   port;
    long    NumPlayers;
    long    MaxPlayers;
    short   unknown2;
} ATTRIB_PACKED;  // 84 bytes

struct WarpPacket
{
    long GameID;
    short Navs;
    long TargetID[20];
} ATTRIB_PACKED;

struct InvMove
{
    long GameID;
    long FromInv;
    long FromSlot;
    long ToInv;
    long ToSlot;
    long Num;
} ATTRIB_PACKED;

struct ItemState
{
    long GameID;
    long BitMask;
    char Enable;
    char Inventory;
    char ItemNum;
} ATTRIB_PACKED;

struct AvatarListItem
{
    AvatarInfo      info;               // 133 bytes (internal struct is 144 bytes)
    AvatarData      data;               // 241 bytes (internal struct is 268 bytes)
} ATTRIB_PACKED;  // 374 bytes

struct GlobalAvatarList
{
    AvatarListItem  avatar[5];      // 5 * 374 bytes
    long            num_galaxies;   // 4 bytes -- 1 to 6 currently hard-coded to 1!
    Galaxy          galaxy[2];      // support only one galaxy!
    // Galaxy           galaxy[4];  // 4 * 84 bytes -- variable length array of galaxies
} ATTRIB_PACKED;

struct GlobalCreateCharacter
{
    long    galaxy_id;              // 4 bytes
    long    character_slot;         // 4 bytes
    long    tutorial_status;        // 4 bytes
    char    account_username[65];   // 65 bytes
    AvatarData avatar;              // 241 bytes
    ShipData ship_data;             // 194 bytes
    char    unknown[27];            // 27 bytes
} ATTRIB_PACKED;  // 539 bytes

struct MasterJoin
{
    long    unknown1;
    long    unknown2;
    long    unknown3;
    long    avatar_id_msb;
    long    avatar_id_lsb;
    long    ToSectorID;
    long    FromSectorID;
    long    PlayerLevel;
    long    unknown8;
    long    unknown9;
    long    unknown10;
    char    ticket[20];
} ATTRIB_PACKED;

// This packet is sent by the Global Server to the Client
// This packet causes the galaxy loading screen to appear.
// The port number should be somewhere within this scructure.
// The "unknown" values appear to control the animation of the "from" and "to" sectors
// on the wait screen.
// The MasterJoin packet is the first packet sent from the
// client to the Global Server once.
//
struct GlobalTicket
{
    long    response_code;
    MasterJoin  join_data;
} ATTRIB_PACKED;

struct ServerRedirect
{
    long    sector_id;
    long    ip_address;
    short   port;
} ATTRIB_PACKED;

struct ChangeBaseAsset
{
	long	GameID;			// buff[12] 4 bytes
	long	BaseAsset;		// buff[16] 4 bytes
	float	Scale;			// buff[20] 4 bytes
	float	HSV[3];			// buff[24] 12 bytes
} ATTRIB_PACKED;

struct Create
{
    long    GameID;                 // this[12] 4 bytes
    float   Scale;                  // this[16] 4 bytes
    short   BaseAsset;              // this[20] 2 bytes
    char    Type;                   // this[22] 1 byte
    float   HSV[3];                 // this[24] 12 bytes
} ATTRIB_PACKED;

struct ServerParameters
{
    float   ZBandMin;               // this[12] 4 bytes
    float   ZBandMax;               // this[16] 4 bytes
    float   XMin;                   // this[20] 4 bytes
    float   YMin;                   // this[24] 4 bytes
    float   XMax;                   // this[28] 4 bytes
    float   YMax;                   // this[32] 4 bytes
    float   FogNear;                // this[36] 4 bytes
    float   FogFar;                 // this[40] 4 bytes
    long    DebrisMode;             // this[44] 4 bytes
    char    LightBackdrop;          // this[48] 1 byte (boolean 1=true 0=false)
    char    FogBackdrop;            // this[49] 1 byte (boolean 1=true 0=false)
    char    SwapBackdrop;           // this[50] 1 byte (boolean 1=true 0=false)
    float   BackdropFogNear;        // this[52] 4 bytes
    float   BackdropFogFar;         // this[56] 4 bytes
    float   MaxTilt;                // this[60] 4 bytes
    char    AutoLevel;              // this[64] 1 byte (boolean 1=true 0=false)
    float   ImpulseRate;            // this[68] 4 bytes
    float   DecayVelocity;          // this[72] 4 bytes
    float   DecaySpin;              // this[76] 4 bytes
    short   BackdropBaseAsset;      // this[80] 2 bytes
    unsigned long SectorNum;        // this[84] 4 bytes
} ATTRIB_PACKED;

struct LoginData
{
    char    unknown40[40];
    char    timestamp[18];      // mm/dd/yy hh:mm:ss, example "10/01/06 16:43:25"
    char    unknown7[7];
} ATTRIB_PACKED;  // 65 bytes

struct Login
{
    MasterJoin  join_data;      // this[16] 64 bytes
    long        TimeSent;       // this[88] 4 bytes
    LoginData   login_data;     // this[96] 65 bytes
    long        TimeReceived;   // this[164] 4 bytes
} ATTRIB_PACKED;

struct SetBBox
{
    float   XMin;               // this[12] 4 bytes
    float   YMin;               // this[16] 4 bytes
    float   XMax;               // this[20] 4 bytes
    float   YMax;               // this[24] 4 bytes
} ATTRIB_PACKED;

struct SetZBand
{
    float   Min;                // this[12] 4 bytes
    float   Max;                // this[16] 4 bytes
} ATTRIB_PACKED;

struct Navigation
{
    long    GameID;
    float   Signature;
    char    PlayerHasVisited;
    long    NavType;
    char    IsHuge;
} ATTRIB_PACKED;

struct CreateAttachment
{
    long    Parent_ID;
    long    Child_ID;
    long    Slot;
} ATTRIB_PACKED;

struct DecalItem
{
    long    Index;
    long    decal_id;
    float   HSV[3];
    float   opacity;
} ATTRIB_PACKED;

#define MAX_DECALS  6   // arbitrary limit

struct Decal
{
    long    GameID;
    short   DecalCount;
    DecalItem Item[MAX_DECALS];
} ATTRIB_PACKED;

struct NameDecal
{
    long    GameID;
    char    Name[32];
    float   RGB[3];
} ATTRIB_PACKED;

struct ColorizationItem
{
    long    metal;
    float   HSV[3];
} ATTRIB_PACKED;

#define MAX_COLORIZATION_ITEMS  10  // arbitrary number

struct Colorization
{
    long    GameID;
    short   ItemCount;
    ColorizationItem item[MAX_COLORIZATION_ITEMS];
} ATTRIB_PACKED;

struct CharacterCreatorAvatarDataFile
{
    AvatarData  avatar;         // 241 bytes
    ShipData   ship;           // 194 bytes
} ATTRIB_PACKED;  // 435 bytes, Avatar1.dat is 564 bytes

struct AvatarDescription // opcode 0x61
{
    unsigned long AvatarID;
    AvatarData  avatar_data;
    char        unknown1[3];
    float       unknown2;
    float       unknown3;
    float       unknown4;
} ATTRIB_PACKED;

struct Subparts // opcode 0xb4
{
    long    GameID;
    long    NumSubParts;
    char    BoneProfession[4];
    long    BassetProfession;
    char    BoneEngine1[11];
    long    BassetEngine1;
    char    BoneEngine2[11];
    long    BassetEngine2;
    char    BoneWing[4];
    long    BassetWing;
} ATTRIB_PACKED;

struct ConstantPositionalUpdate
{
    long    GameID;             // this[12] 4 bytes
    float   Position[3];        // this[16] 12 bytes
    float   Orientation[4];     // this[28] 16 bytes
} ATTRIB_PACKED;

struct FormationPositionalUpdate
{
    long	TargetID;			// this[16] 4 bytes
    long    LeaderID;           // this[12] 4 bytes
    float   Position[3];        // this[20] 12 bytes
} ATTRIB_PACKED;

struct RequestTarget
{
    long    GameID;             // this[12] 4 bytes
    long    TargetID;           // this[16] 4 bytes
} ATTRIB_PACKED;

struct SetInterface
{
	long UIChange;
	long UIType;
} ATTRIB_PACKED;

struct SetTarget
{
    long    GameID;             // this[12] 4 bytes
    long    TargetID;           // this[16] 4 bytes
} ATTRIB_PACKED;

struct ActionPacket
{
    long    GameID;             // this[12] 4 bytes
    long    Action;             // this[16] 4 bytes
    long    Target;             // this[20] 4 bytes
    long    OptionalVar;        // this[24] 4 bytes
} ATTRIB_PACKED;

struct ClientChat
{
    long    GameID;             // this[12] 4 bytes
    char    Type;               // this[22] 1 byte
    short   Size;               // this[20] 2 bytes = strlen(String) + 1
    char    String[1];          // variable length string
} ATTRIB_PACKED;

struct ClientSetTime
{
    long    ClientSent;
    long    ServerReceived;
    long    ServerSent;
} ATTRIB_PACKED;

struct VerbRequest
{
    long    SubjectID;
    long    ObjectID;
    long    Action;
} ATTRIB_PACKED;

struct CameraControl
{
    long    Message;
    long    GameID;
} ATTRIB_PACKED;

struct LogoffRequest
{
    long    PlayerID;           // this[12] 4 bytes
	long	LogOutType;
} ATTRIB_PACKED;

struct TriggerEmote
{
    long    GameID;
    long    Emote;
} ATTRIB_PACKED;

struct ChatStream
{
	long GameID;
	char Unknown1;				// I can't tell what this does (It's always 0x01.  Maybe for byte-alignment?)
	short ChatSize;				// The size of the rest of the packet + 2 additional bytes (Target as mentioned below?)
	char message[1];			// Variable length string
} ATTRIB_PACKED;

struct NotifyEmote
{
    long    GameID;
    long    Emote;
} ATTRIB_PACKED;

struct OptionPacket
{
    long    GameID;             // this[12] 4 bytes
    long    OptionType;         // this[16] 4 bytes
    unsigned char OptionVar;    // this[20] 1 byte
} ATTRIB_PACKED;

struct SelectTalkTree
{
    long    PlayerID;
    unsigned char Selection;
} ATTRIB_PACKED;

struct ClientChatRequest
{
    long    PlayerID;
    long    unknown1;
    short   string_length;
} ATTRIB_PACKED;

struct ClientSkillsRequest
{
	long PlayerID;
	long unknown1;
} ATTRIB_PACKED;

struct StarbaseAvatarChange
{
    long    AvatarID;
    long    RoomType;
    float   Orient;
    float   Position[3];
    long    ActionFlag;
} ATTRIB_PACKED;

struct StarbaseAvatarChange_S2C
{
    long    AvatarID;
    float   Orient;
    float   Position[3];
    long    ActionFlag;
    long    Room;
} ATTRIB_PACKED;

struct StarbaseRoomChange
{
    long    AvatarID;
    long    NewRoom;
    long    OldRoom;
} ATTRIB_PACKED;

struct StarbaseRequest
{
    long    PlayerID;
    long    StarbaseID;
    char    Action;
} ATTRIB_PACKED;

#define RELATIONSHIP_ATTACK     0
#define RELATIONSHIP_SHUN       1
#define RELATIONSHIP_FRIENDLY   2
#define RELATIONSHIP_ADORATION  3

struct Relationship
{
    long    ObjectID;
    long    Reaction;
    char    IsAttacking;
} ATTRIB_PACKED;

struct ObjectEffect             // opcode 0x09
{
    char    Bitmask;            // bitfield of flags
    long    GameID;
    short   EffectDescID;
    long    EffectID;           // bit 0
    unsigned long TimeStamp;    // bit 1
    short   Duration;           // bit 2
    float   Scale;              // bit 3
    float   HSVShift[3];        // bit 4,5,6
} ATTRIB_PACKED;

struct ObjectToObjectEffect             // opcode 0x0B
{
    u16		Bitmask;            // 4 flags for condional fields
    long    GameID;
	long	TargetID;
    u16		EffectDescID;
	const char	*Message;
    // the following fields are not always present, inclusion depends on bitmask
    long    EffectID;           // bit 0 mask 0x0001
    unsigned long TimeStamp;    // bit 1 mask 0x0002
	u16		Duration;			// bitmask[2] 2 bytes (time is in milli seconds)
	float	TargetOffset[3];	// bitmask[3] 12 bytes
	u16		OutsideTargetRadius;// bitmask[4] 2 bytes
	u16		unused;				// bitmask[5] 2 bytes
	float	Scale;				// bitmask[6] 4 bytes
	float	HSVShift[3];		// bitmask[7] 12 bytes
	float	Speedup;			// bitmask[8] 4 bytes
} ATTRIB_PACKED;

struct InitRenderState			// opcode 0x2f
{
	long	GameID;
	unsigned long RenderStateID;
} ATTRIB_PACKED;

struct ActivateRenderState      // opcode 0x30
{
    long    GameID;                 // this[12] 4 bytes
    unsigned long RenderStateID;    // this[20] 4 bytes
} ATTRIB_PACKED;

struct SimplePositionalUpdate
{
    long    GameID;                 // this[12] 4 bytes
    unsigned long TimeStamp;        // this[16] 4 bytes
    float   Position[3];            // this[20] 12 bytes
    float   Orientation[4];         // this[32] 16 bytes
    float   Velocity[3];            // this[48] 12 bytes
} ATTRIB_PACKED;

struct PlanetPositionalUpdate
{
    long    GameID;                 // this[12] 4 bytes
    unsigned long TimeStamp;        // this[16] 4 bytes
    float   Position[3];            // this[20] 12 bytes
    long    OrbitID;                // this[32] 4 bytes
    float   OrbitDist;              // this[36] 4 bytes
    float   OrbitAngle;             // this[40] 4 bytes
    float   OrbitRate;              // this[44] 4 bytes
    float   RotateAngle;            // this[48] 4 bytes
    float   RotateRate;             // this[52] 4 bytes
    float   TiltAngle;              // this[56] 4 bytes
} ATTRIB_PACKED;

struct ComponentPositionalUpdate
{
    struct  SimplePositionalUpdate simple;  // this[12] 48 bytes
    float   ImpartedDecay;                  // this[68] 4 bytes
    float   TractorSpeed;                   // this[72] 4 bytes
    long    TractorID;                      // this[76] 4 bytes
    long    TractorEffectID;                // this[80] 4 bytes
} ATTRIB_PACKED;

struct AdvancedPositionalUpdate
{
    short   Bitmask;                // flags for condional fields
    long    GameID;                 // this[12] 4 bytes
    unsigned long TimeStamp;        // this[16] 4 bytes
    float   Position[3];            // this[20] 12 bytes
    float   Orientation[4];         // this[32] 16 bytes
    unsigned long MovementID;       // this[100] 4 bytes
    // the following fields are not always present, inclusion depends on bitmask
    float   CurrentSpeed;           // this[48] 4 bytes     bit 0  0x0001
    float   SetSpeed;               // this[52] 4 bytes     bit 1  0x0002
    float   Acceleration;           // this[56] 4 bytes     bit 2  0x0004
    float   RotY;                   // this[60] 4 bytes     bit 3  0x0008
    float   DesiredY;               // this[64] 4 bytes     bit 4  0x0010
    float   RotZ;                   // this[68] 4 bytes     bit 5  0x0020
    float   DesiredZ;               // this[72] 4 bytes     bit 6  0x0040
    float   ImpartedVelocity[3];    // this[76] 12 bytes    bit 7  0x0080
    float   ImpartedSpin;           // this[88] 4 bytes     bit 7  0x0080
    float   ImpartedRoll;           // this[92] 4 bytes     bit 7  0x0080
    float   ImpartedPitch;          // this[96] 4 bytes     bit 7  0x0080
    unsigned long UpdatePeriod;     // this[104] 4 bytes    bit 8  0x0100
} ATTRIB_PACKED;

struct EquipUse
{
    long    GameID;      // 4 bytes
    char	InvNum;      // 1 bytes
	char	InvSlot;     // 1 bytes
} ATTRIB_PACKED;

struct AbilityUse
{
    long    GameID;      // 4 bytes
    long	UnKnown;     // 4 bytes
	long	Ability;     // 4 bytes
} ATTRIB_PACKED;

struct StarbaseSet
{
    long    StarbaseID;
    char    Action;
    char    ExitMode;
} ATTRIB_PACKED;

struct ServerHandoff
{
    MasterJoin  join;
    char        variable_data[128];
} ATTRIB_PACKED;

struct ShipInfo
{
    long    hull;
    long    profession;
    long    engine;
    long    wing;
    float   Position[3];
    float   Orientation[4];
} ATTRIB_PACKED;

struct CharacterDatabase
{
    AvatarInfo      info;               // 133 bytes
    AvatarData      avatar;             // 241 bytes
    ShipData        ship_data;          // 194 bytes
    ShipInfo        ship_info;
} ATTRIB_PACKED;

struct CTARequest
{
	long SourceID;
	long TargetID;
	long Action;
} ATTRIB_PACKED;

struct MovePacket
{
    long GameID;
    char type;
} ATTRIB_PACKED;

struct SkillAction
{
	long	GameID;
	int		SkillPoints;
	short	SkillID;
} ATTRIB_PACKED;

struct SkillUse // Opcode 0x58
{
    long GameID;
    long Action;
    long AbilityIndex;
} ATTRIB_PACKED;

struct MissionDismissal
{
	long PlayerID;
	long MissionID;					// Could be 2 or 4 bytes, the 1st 2 bytes are always 0 from my observations
} ATTRIB_PACKED;

struct MVASHandoff
{
	long	player_id;
	long	port;
} ATTRIB_PACKED;


// LoungeNPC Structure

struct StationData {
	int	StationType;
	int	RoomNumber;
} ATTRIB_PACKED;

struct StationRooms {
	int RoomNumber;
	int	RoomStyle;
	float FogNear;
	float FogFar;

	int FogRed;
	int FogGreen;
	int FogBlue;
} ATTRIB_PACKED;

// Term Number

struct StationTerms {
	int	RoomNumber;
	int Location;
	int TermType;
	int Unknown;
} ATTRIB_PACKED;

// NPC Number

struct StationNPC {
	int RoomNumber;
	int	Location;
	int NPCID;
	int BoothType;
	int Unknown1;
	int Unknown2;
	struct AvatarData Avatar;
} ATTRIB_PACKED;

struct StationLounge {
	struct StationData	Station;
	struct StationRooms	Rooms[5];
	int					NumTerms;
	struct StationTerms	Terms[15];
	int					NumNPCs;
	StationNPC	NPC[30];
} ATTRIB_PACKED;

struct ManufactureData
{
	long GameID;
	long Data;
} ATTRIB_PACKED;

struct ManufactureTechLevelFilter
{
    long GameID;
    char Enable;
    long BitField;
} ATTRIB_PACKED;

#endif // _PACKET_STRUCTURES_H_INCLUDED_

