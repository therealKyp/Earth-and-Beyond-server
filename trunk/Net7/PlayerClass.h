// PlayerClass.h

#ifndef _PLAYER_CLASS_H_INCLUDED_
#define _PLAYER_CLASS_H_INCLUDED_

#define MAX_NUMBER_OF_SPACE_SECTORS 100
#define MAX_NUMBER_OF_GALAXY_OBJECTS 65536 //this refers to the database UID of the object, which can't change. We could use a std::map here but this method is quicker and guaranteed static
#define MAX_MISSIONS 12
#define ATTRIBUTE_ENABLE 0x00
#define ATTRIBUTE_DIS_INGROUP 0x01 //disabled - 'already in group'
#define ATTRIBUTE_DIS_TOOFAR 0x02 //disabled - 'Too Far'

#define UDP_BUFFER_SEND_SIZE 16384

#define CIRCULAR_CACHE_SIZE 24576 //this buffer stores previous packets
#define CIRCULAR_PACKET_ARRAY_SIZE 16
#define PERIODIC_CACHE_SEND_SIZE 1472 //max without fragmentation - see if this improves UDP comms
#define MAXIMUM_PACKET_CACHE 4096
#define TALKTREE_BUFFER_SIZE 1024

#define	MAX_WEAPON_FIRE_PER_TICK 10   // maximum displayed weapon fire per tick

#define MAX_NAVS_DECOS	768			  //this refers to how many navs and decos we can track the ranges from for the arrival stuff - any decos above that number won't get tracked.
#define TRIGGER_RANGE_1	30000
#define TRIGGER_RANGE_2	10000
#define TRIGGER_RANGE_3	2500

#include "Net7.h"
#include "AuxClasses\AuxPlayerIndex.h"
#include "AuxClasses\AuxShipIndex.h"
#include "AuxClasses\AuxManufacturingIndex.h"
#include "ObjectClass.h"
#include "Equipable.h"
#include "MissionManager.h"
#include "Stats.h"
#include "ItemList.h"
#include "MessageQueue.h"
#include "TalkTreeParser.h"
#include "PlayerEffects.h"
#include "PlayerBuffs.h"
#include "AbilityBase.h"

class Player;
class Stats;
class Connection;

typedef std::vector<Player*> PlayerList;
typedef std::vector<long> CompletedMissionIDList;

#define GRAIL_AFFINITY (1<<0) //TODO: integrate this into equipable, we need a custom item handling system

enum experience_type { XP_COMBAT, XP_EXPLORE, XP_TRADE };

typedef enum
{
	//Basic ship values
	ID_SCAN_RANGE,
	ID_IMPULSE,
	ID_SIGNATURE,

	//weapon damage
	ID_BEAM_DAMAGE,
	ID_PROJ_DAMAGE,
	ID_MISSILE_DAMAGE,

	//Weapon damage modifiers
	ID_BEAM_MULTIPLIER,
	ID_PROJ_MULTIPLIER,
	ID_MISSILE_MULTIPLIER,
	
	//Warp things
	ID_WARP_ENERGY,
	ID_WARP_RECOVERY,

	//weapon accuracy
	ID_BEAM_ACCURACY,
	ID_PROJ_ACCURACY,
	ID_MISSILE_ACCURACY,

	//deflects
	ID_IMPACT_DEFLECT,
	ID_EXPLOSIVE_DEFLECT,
	ID_PLASMA_DEFLECT,
	ID_ENERGY_DEFLECT,
	ID_EMP_DEFLECT,
	ID_CHEMICAL_DEFLECT,
	ID_PSIONIC_DEFLECT,

	//Abilities/Skills
	ID_REACTOR_RECHARGE_RATE,
	ID_SHIELD_RECHARGE_RATE,
	ID_CRITICAL_TARGETING,
	ID_DAMAGE_CONTROL,
	ID_WARP_SPEED,
	ID_NEGOTIATE,
	ID_COMBAT_TRANCE,
	ID_SCAN,
	ID_NAVIGATE_IMPLUSE,
	ID_PROSPECT,
	ID_TRACTORBEAM
} BASE_STAT_IDS;

struct VerbSlot  //this structure details a verb storage slot
{
    short       verb_id;
	bool	    active;
    float       activate_range;
} ATTRIB_PACKED;

struct CacheIndex
{
    long buffer_index;
    long packet_num;
    long packet_size;
} ATTRIB_PACKED;

class Player : public Object
{
    friend class Equipable;
	friend class Stats;

public:
    Player(long object_id);
    Player();
    virtual ~Player();

    void        SetGroupID(long group_id)       { m_GroupID = group_id;}
    long        GroupID()                       { return (m_GroupID); }
	void		SetAcceptedGroupInvite(bool ac) { m_AcceptedGroupInvite = ac; }
	bool		AcceptedGroupInvite()			{ return (m_AcceptedGroupInvite); }

    long        CharacterID()                   { return (m_CharacterID); }
    void        SetCharacterID(long char_id);
    long        CharacterSlot()                 { return (m_CharacterSlot); }
    void        SetCharacterSlot(long slot)     { m_CharacterSlot = slot; }
    AuxPlayerIndex  *PlayerIndex()              { return (&m_PlayerIndex); }
    AuxShipIndex    *ShipIndex()                { return (&m_ShipIndex); }
    AuxManufacturingIndex *ManuIndex()          { return (&m_ManuIndex); }
    long        ManuID()                        { return (GameID() | MANU_TAG); }
    Group      *GetGroup()                      { return (m_Group); }
    void        SetGroup(Group *group)          { m_Group = group; }

    //Helper methods for data
    long        CombatLevel()                   { return (PlayerIndex()->RPGInfo.GetCombatLevel()); }
    long        ExploreLevel()                  { return (PlayerIndex()->RPGInfo.GetExploreLevel()); }
    long        TradeLevel()                    { return (PlayerIndex()->RPGInfo.GetTradeLevel()); }
    long        TotalLevel()                    { return (CombatLevel() + ExploreLevel() + TradeLevel()); }
    long        Race()                          { return (m_Database.ship_data.race); }
    long        Profession()                    { return (m_Database.ship_data.profession); }
    long        ClassIndex()                    { return (Race() * 3 + Profession()); }
    long        AdminLevel()                    { return (htonl(m_Database.info.admin_level)); }

	float		GetDamageAbsorb()				{ return m_DamageAbsorb; }
	void		SetDamageAbsorb( float Damage ) { m_DamageAbsorb = Damage; }

    long        FromSector()                    { return m_FromSectorID; }
	long		StargateDestination()			{ return m_StargateDestination; }
	void		SetStargateDestination(long s)	{ m_StargateDestination = s; }


    long        TractorRange();
    long        ProspectRange();

    void        SetDebugPlayer()                { m_DebugPlayer = true; }
    bool        DebugPlayer()                   { return (m_DebugPlayer); }

	void		SetupAbilities();
    void        ResetPlayer();
    void        SectorReset();
    void        Remove();
    void        FirstLogin();
    void        SendMOTD();
    void        SetAccountUsername(char *name);
    char       *AccountUsername()               { return (m_AccountUsername); }

	void		SetGameIndex(u32 game_index)	{ m_GameIndex = game_index; }
	u32			GetGameIndex()					{ return m_GameIndex; }

    void        SetDatabase(CharacterDatabase &database);
	bool		IsSubscribed(long channel_id)	{ if (channel_id < 32) return m_ChannelSubscription[channel_id]; else return false; }

    ObjectManager* GetObjectManager();

    void        MarkForRemoval();
    bool        ToBeRemoved()                   { return (m_Remove); }
	void		SetLoginStage(long stage)		{ m_LoginStage = (u8)stage; }
	long		GetLoginStage()					{ return (long)m_LoginStage; }
	void		HandleLoginStage2();
	void		IssueTCPBuffer();

    long       *ObjectRangeList()               { return (m_ObjRangeList); }
    long       *ResourceSendList()              { return (m_ResourceSendList); }
    long       *ExposedNavList();   
    long       *ExploredNavList();  
	void		ResetRangeLists();
    bool        IsInSameSector(Player *p);
	void		ExposeDecosOn(Object *obj);
	void		ExposeDecosOff(Object *obj);
	u32		   *GetSectorPlayerList();

	void		SetDistressBeacon(bool Status);
	void		TowToBase();



    CharacterDatabase *Database()               { return (&m_Database); }

	void	    SendVaMessage(char *msg, ...);
    void        SendVaMessageC(char colour, char *msg, ...);
    void        SendMessageString(char *msg, char color=5, bool log=true);

    void        SetProspecting(bool p)          { m_Prospecting = p; };
    bool        Prospecting()                   { return (m_Prospecting); };

    bool        HijackObject(Object *obj);
	char	  * GetHijackeeName();

    void        SetMyDebugPlayer(Player *p)     { m_MyDebugPlayer = p;}
    void        DebugPlayerDock(bool flag);

    //Connection Methods
    void        SetUDPConnection(UDP_Connection *conn)      { m_UDPConnection = conn; }
    long        PlayerIPAddr()                  { return (m_Player_IPAddr); }
    void        SetHandoffReceived(bool value)  { m_UDPHandoffReceived = value; }
    void        WaitForHandoffReceived();
    void        LoginFailed();
    void        PacketCache();
    void        SendPacketCache();
    void        CacheOpcode(short opcode, unsigned char *data, u16 length);
	void		CommitCacheToQueue();
	//static DWORD WINAPI thread_start_routine (void *arg)
    static DWORD WINAPI Player::MsgSendThread(void *Param);
	void		PulsePlayerInput();
	void		FinishLogin();

    //starbase methods
    void        SendStarbaseAvatarList();
    void        HandleStarbaseAvatarChange(long sector_id, StarbaseAvatarChange *change);
    void        HandleStarbaseRoomChange(long sector_id, StarbaseRoomChange *change, int aflag=1);    
    void        BroadcastPosition();
    void        SendAvatarListToPlayer();
    void        SendStarbaseAvatarChange(Player *p);

    void        SetActionFlag(long flag)        { m_ActionFlag = flag; }
    long        ActionFlag()                    { return (m_ActionFlag); }
    bool        HavePosition()                  { return (m_HavePosition); }
    void        SetHavePosition()               { m_HavePosition = true; }

    void        LogDockCoords();                 

    char       *GetRank();

    //ship send methods
    void        SendShipData(Player *player_to_send_to);
    void        SendSubparts(Player *player_to_send_to);
    void        SendShipColorization(Player * player_to_send_to, int count);
    void        SendPlayerInfo();
    void        SendShipInfo(long, long);
    short       GetHullNum();

    void        SendLoginShipData();
    void        SendLoginSubparts();
    void        SendLoginShipColorization(int count);

    //aux methods
	void	    SendAuxShip(Player * other = 0);
	void	    SendAuxShipExtended();

	void	    SendAuxPlayer();
	void	    SendAuxPlayerExtended();

	void	    SendAuxManu(bool TwoBitFlags = false);

    void        SetActive(bool flag);

    //position updates
    void        SendLocationAndSpeed(bool include_player);
    void        SetPlayerUpdate(short n)        { m_SetUpdate = n; }
    short       PlayerUpdateSet()               { return (m_SetUpdate); }
	void		SendStartMovementRefresh();

    //MVAS methods:
    u16			UpdatePositionFromMVAS(float *position, float *orientation, bool orientation_sent);
    bool        ReceivedMVAS()                  { return (m_ReceivedMVAS); }
    void        SetMVASIndex(long MVAS_index)   { m_MVAS_index = MVAS_index;}
    long        MVASIndex()                     { return (m_MVAS_index); }
    bool        UsingMVAS()                     { return (m_MVAS_index == -1) ? false : true; }
    u16			Frequency()                     { return (m_MVAS_frequency); }
    void        SetFrequency(u16 freq)			{ m_MVAS_frequency = freq; }
    unsigned char *GetUDPBuffer()               { return (m_UDPSendBuffer); }
    
    //Movement methods
    void        SetVelocity(float velocity);
    float       Velocity()                      { return (m_Velocity); }
    void        Move(int type);
    void        MoveToward(Object *obj, float speed);
    void        CalcNewPosition(unsigned long current_tick, bool turn = false);
    void        SetInSpace(bool in_space)       { m_InSpace = in_space; }
    bool        InSpace()                       { return (m_InSpace); }
    void        SectorLogin();
    void        CheckNavs();
    bool        Following()                     { return (m_FollowObject); }
    void        StoreDockingCoords(float *position, float *heading);
    bool        RestoreDockingCoords();
    void        SetOrient(float o)              { m_Orient = o; }

    void        SetNearestNav(Object *obj)      { m_NearestNav = obj; }
    Object     *GetNearestNav()                 { return (m_NearestNav); }

    //warp methods
    void        SetupWarpNavs(short navs, long *target_id);
    void        PrepareForWarp();
    void        StartWarp();
    void        TerminateWarp(bool player_forced = false);
	void		TerminateWarpGroup(bool player_forced = false);
    void        UpdateWarpNavigation();
    void        FinalWarpReset();
    bool        WarpDrive()                     { return (m_WarpDrive); }
    void        SetWarp()                       { m_WarpDrive = true; }
    void        CheckWarpNavigation();

    //rangelist handling
    void        UpdatePlayerVisibilityList();
    bool        PlayerInRangeList(Player *p_check);
    void        AddPlayerToRangeList(Player *p);
    void        RemovePlayerFromRangeList(Player *p);
    void        RemoveFromAllSectorRangeLists(); //this removes the presence of the player from all the lists in the current sector
    bool        IsClickedBy(Player *p);
	void		BlankRangeList(); //this removes the player from visibility

    //FX sending
    void        SendTractorComponentRL(Object *obj, float decay, float tractor_speed, long article_id, long effect_id, long timestamp);
    void        SendResourceNameRL(long article_UID, char *raw_name);
    void        SendResourceRemoveRL(long article_UID);
    void        SendContrailsRL(bool contrails);
    void        SendEffectRL(long target_id, char *message, long effect_type, long effectUID, long timestamp, short duration = 0);
    void        RemoveEffectRL(long effect_UID);
    void        SendObjectToObjectEffectRL(ObjectToObjectEffect *obj_effect, bool weapon_fire = false);
    void        SendObjectEffectRL(ObjectEffect *object_effect);
    void        SendToRangeList(short opcode, unsigned char *data, size_t length, bool weapon_fire = false);
    void        SendToGroup(short opcode, unsigned char *data, size_t length);
    void        SendToSector(short opcode, unsigned char *data, size_t length);
    void        SendObjectCreateRL(long article_UID, float scale, short basset, int type);
    void        SendRelationshipRL(long article_UID, long relationship, long is_attacking);
    void        SendObjectLinkedEffectRL(short bitmask, long UID, long effectID, short effectDID, long effect_time);
	void		SendEffect(ObjectEffect *object_effect);

    //XP
    float       AwardXP(experience_type xp_type, long xp_gain, bool skip_group = false);
    void        AwardNavExploreXP(Object *obj);
    void        AwardCombatXP(char *message, long xp, bool skip_group = false);
    void        AwardExploreXP(char *message, long xp, bool skip_group = false);
    void        AwardTradeXP(char *message, long xp, bool skip_group = false);
    void        AddMOBDestroyExperience(short mob_level, char *mob_name);
    short       CalculateMOBXP(short mob_level);

	void		AwardFaction(long faction_id, long faction_change);

    //inventory methods
    void        SendItemBase(long);
    long        CargoItemCount(long ItemID);
    bool        CanCargoAddItem(_Item *);
    bool        CanCargoAddItem(long ItemID, long Stack = 1, float Quality = 1.0f);
    long        CargoAddItemCount(long ItemID, float Quality = 1.0f);
    int         CargoAddItem(long ItemID, long Stack, int Tradable = 0);
    int         CargoAddItem(_Item *);
	bool		QualityCalculator(_Item * myItem);
    void        CargoRemoveItem(long ItemID, long Count);
	static void	CheckStack(int MoveNum, _Item * From, _Item * To);
	void		AwardCredits(u64 credits, long XP_earned, bool screen_message = false);

    void        SetPrices();
    void        ClearPrices();

    //save/load status
    void        SavePosition();
    bool        LoadPosition();
    bool        ReadSavedData();
    void        ReInitializeSavedData();
	void		ReloadSavedData();
    void        SaveData(bool reset_data = false);
    bool        SaveDatabase();
	void		UpdateDatabase();
    bool        WriteCompletedMissions(FILE *f);
    bool        ReadCompletedMissions(FILE *f);
	void		SaveNewSkillLevel(long skill_id, long skill_level);
	void		SaveInventoryChange(long slot);
	void		SaveVaultChange(long slot);
	void		SaveXPBarLevel(long xp_type, float xp_bar);
	void		SaveCreditLevel();
	void		SaveEquipmentChange(long slot, _Item *item);
	void		SaveAmmoChange(long slot, _Item *ammo);
	void		SaveAdvanceMission(long slot);
	void		SaveMissionFlags(long slot);
	void		SaveAdvanceLevel(long xp_type, long level);
	void		SaveHullLevelChange(float new_hull_level);
	void		SaveHullUpgrade();
	void		SaveFactionChange(long faction_id, float new_value, long faction_order = -1);
	void		CreatePositionSave();
	void		CompleteMission(long mission_id, long completion_flags);
	void		WipeCharacter();
	void		WipeFactions();
	void		DeleteAllAvatarRecords();
	void		SaveDiscoverNav(long object_uid);
	void		SaveExploreNav(long object_uid);
	void		LoadExploredNavs(sql_query_c *query);
	void		SetupFactions(sql_query_c *query, bool force);
	void		SaveSkillPoints();
	void		LoadMissionStatus(sql_query_c *query);
	void		SaveAmmoLevels();
	void		SaveRegisteredStarbase();
	void		SaveEnergyLevels();
	void		SetHullUpgrade();

    //skills
    void        HandleSkillAction(unsigned char *data);
	void		SkillUpdateStats(int SkillID);
    void        UpdateSkills();
    void        UpgradeSkill(int);
    void        LevelUpForSkills();
	void		SkillsList();
    void        ResetWeaponMounts();
	void		ResetDeviceMounts();
	long		ConvertAbilityToBaseSkill(long &level, long ability);
	void		AddScanSkill(float new_factor);
	void		SetCurrentSkill(AbilityBase* CurrentSkill = NULL) { m_CurrentSkill = CurrentSkill; };
	AbilityBase*GetCurrentSkill() { return m_CurrentSkill; };

	// Abilities
	void		HandleSkillAbility(unsigned char *data);
	void		AbilityRemove(int AbilityID);
	int			GetStealthLevel()							{ return m_StealthLevel; };
	void		SetStealthLevel(int NewStealth)				{ m_StealthLevel = NewStealth; };
	
    //sector login camera
    void        SetLoginCamera(long index, long obj_id)     { m_CameraSignal = index; m_CameraID = obj_id; }
    void        SendLoginCamera();

    long	    Hijackee()                                  { return (m_Hijackee); }
    void        SetHijackee(long obj_id)                    { m_Hijackee = obj_id; }

    //Combat & Damage
    void        ShipDamage(long source_id, float weapon_damage, short type);
    void        AttackMusicUpdate(bool update, long mob_id);
    void        FireAllWeapons();
    void        RemoveHull(float hull_dmg, Object *mob);
    void        ImmobilisePlayer();
    void        RemobilisePlayer();
    bool        FireEnergyCannon(ItemInstance *item);
    float       CaughtInEnergyBlast(long source_id, float blast_range);
    short       AttacksThisTick()                           { return m_AttacksThisTick; }
    void        ResetAttacksThisTick()                      { m_AttacksThisTick = 0; }
    void        IncAttacksThisTick()                        { m_AttacksThisTick++; };
    float       CalcDamage(int weapon_damage, int subcat, bool *critical);
	bool		HasCombatImmunity()							{ return m_CombatImmunity; };

    // energy and shield
    float       BaseEnergyRecharge()                        { return (m_Stats.GetStat(STAT_ENERGY_RECHARGE) / m_Stats.GetStat(STAT_ENERGY)) / 1000.0f; }
    float       BaseShieldRecharge()                        { return (m_Stats.GetStat(STAT_SHIELD_RECHARGE) / m_Stats.GetStat(STAT_SHIELD)) / 1000.0f; }
    void        SetBaseEnergyRecharge(float Recharge)       { m_ReactorChargeRate = Recharge; }
    void        SetBaseShieldRecharge(float Recharge)       { m_ShieldChargeRate = Recharge; }
    float	    GetEnergy();
	float	    GetEnergyValue();
	float	    GetShield();
	float	    GetShieldValue();
	float		GetShieldLevel()							{ return GetShieldValue(); };
	void	    DrainReactor(unsigned long, float);	
    void        RemoveEnergy(float);
    void        RemoveShield(float);
    void        EnergyUpdate(unsigned long, float, float);
    void        ShieldUpdate(unsigned long, float, float);
	void	    RechargeReactor();
    void        RechargeShield();

    //manufacturing
    void        HandleManufactureTerminal(unsigned char *data);
    void        HandleManufactureCategorySelection(unsigned char *data);
    void        HandleManufactureSetItem(unsigned char *data);
    void        HandleRefineSetItem(unsigned char *data);
    void        HandleManufactureAction(unsigned char *data);
    void        HandleManufactureLevelFilter(unsigned char *data);
    void        ManufactureTimedReturn(long);

    //missions
    long        AssignMission(long mission_id); //the mission id here is the 'Mission ID =' from the XML/DB.
    bool        CheckMissions(long object_id, long item_id, long npc_id, completion_node_type completion_type);
    bool        CheckForNewMissions(long object_id, long param_1, long npc_id);
    void        SetGrailAffinity(bool flag)             { flag ? m_MissionFlags |= GRAIL_AFFINITY : m_MissionFlags &= (0xFFFFFFFF - GRAIL_AFFINITY); }
    bool        GrailAffinity()                         { return (m_MissionFlags & GRAIL_AFFINITY); }
    bool        CheckMissionValidity(long target);
    void        MissionObjectScan(Object *obj);
    void        ShipUpgrade(long upgrade);
    void        MissionDismiss(long mission_id);
	void		CheckMissionMOBKill(long MOB_id);
	void		CheckMissionSkillUse(long base_skill, long level);
	void		CheckMissionArrivedAt();
	void		CheckMissionArrivedAt(Object *obj);
	void		CheckMissionRangeTrigger(Object *obj, long range);

	void		CheckArrival();
	int			GetVenderBuyPrice(int ItemID);

    //verbs
    void        BlankVerbs();
    void        UpdateVerbs(bool force_update=false);
    bool        AddVerb(short verb_id, float verb_activate_range);
    void        OnTargeted(Player *player);
	void		WormHole(int sector_id);		// Wormhole to a sector

    //prospecting & looting
    void        LootItem(long slot);
	bool		Looting() {return m_Looting;};
	void		SetLooting(bool answer) {m_Looting = answer;};
    void        MineResource(long slot);
    void        AbortProspecting(bool recharge_reactor, bool abort_both_beams);
    void        PullOreFromResource(Object *obj, long stack_val, long slot, long effect_UID, long incomplete);
    void        TakeOreOnboard(Object *obj, long article_UID);
    long        GetCargoSlotFromItemID(int StartLocation, int ItemID);
    void        RemoveProspectNodes();

    //timing
    void        CheckEventTimes(unsigned long current_tick);
    void        PopulateTimedCall(TimeNode *this_node, broadcast_function func, long time_offset, Object *obj, long i1 = 0, long i2 = 0, long i3 = 0, long i4 = 0, char *ch = 0, float a = 0.0f);

    void        ChangeMountBoneName(long weapon_id, char *mount_name);

    //connection/message handling
    void        AddMessage(short opcode, short length, unsigned char *data);
    void        HandleClientOpcode(short opcode, short bytes, unsigned char *data);
    void        SetPlayerPortIP(short port, long ip_addr);

	//faction interaction
	float		GetFactionStanding(Object *obj);


private:
	// NPC Talk Trees
	bool		CheckTalkTree(int Selection);
	bool		NPCTradeItems();
	long GenerateTalkTree(TalkTree *tree, int tree_node_id);
	u16 GetNodeFlags(TalkTree *tree, int tree_node_id);
	long ParseTalkTokens(char *write_buffer, char *input_buffer);
	void		SendPIPAvatar(long game_id, long avatar_id, bool actual_use);

    //MVAS methods
    void        UpdateOrientation();
    void        UpdateLastPosition(unsigned long current_tick);

    //Movement methods
    bool        CheckUpdateConditions(unsigned long current_tick);
    void        CalcNewHeading(float tdiff, bool turn);
    float       CalcVelocity(float tdiff);
    void        SendToVisibilityList(bool include_player);
    void        UpdatePositionInfo();
    float       FollowTarget();
    void        StopFollowing();
	void		CheckArrivalTriggers();

    //RangeList methods
    void        SetClickedBy(Player *p);
    
    //warp methods
    void        BlankWarpNavs();
    void        InitiateWarpBroadcast();
    void        StartWarpBroadcast();
    void        SendEndWarpBroadcast();
    void        FreeWarpDrain();

    //skills
    long        SkillLevelRequirement(long);
    void        AddWeapon(long weapon_id);
    void        NeatenUpWeaponMounts();

    //manufacturing
    void        BuildRefineList();
    void        CheckItemRequirements();
    void        SetRefineIterations();
    void        ResetManuItems();
    bool        HasComponents(long = 1);
    void        RemoveComponentsFromInventory(long = 1);

    //missions
    void        AdvanceMission(long mission_slot, long stage); //internal mission is the player mission ref, 0 to 19.
    bool        NPCTalkTree(long mission_slot, long response, completion_node_type completion_type = NULL_NODE);
    void        ProposeMissionTree(long mission_id, long response);
	void		ProposePushMissionTree(long mission_id, long response);
    void        RemoveMission(long mission_slot);
    void        EndStageReward(long mission_id, long stage);
    bool        CheckEndStageConditions(AuxMission *m);
	bool		CheckStageCompletionNodes(long mission_id, long stage, long npc_id, Object *obj = (0), long param_data = 0, completion_node_type completion_type = NULL_NODE);
    long        GetSlotForMission(long mission_id);
	bool		CheckMissionCompleted(long mission_id);
	bool		CheckMissionStarted(long mission_id);
	int         UpdateMissionData(int offset, int length, long &missionData);
	bool		CheckSpaceNPC(long response);

    //verbs
    void        CloseInterfaceIfRequired(short verb_id);

    //prospecting & looting
    float       GetEnergyPerOre(ItemBase *item);
    u32         GetResourceDrainTime(long energy, long stack_val);
    bool        TractorInUse();
    u32         TractorCompletion();
    void        UseTractorBeam(Object *obj, short stack_val, ItemBase * contents, long effect_UID);
    short       CalcMiningXP(int stack, short level);
    void        AddMiningExploreExperience(short XP_earned, short stack, char *raw_name);
    bool        CheckMiningConditions(long slot, float reactor_energy);
    bool        CheckLootConditions(long slot);
    bool        CheckInventoryForRoom(long ItemID, int Stack);
	void		ChangeProspectSkill(float new_factor);
	void		ChangeTractorBeamSpeed(float new_factor);

    //connection methods for incomming opcodes (from client)
    void        HandleLogin(unsigned char *data);                          // opcode 0x02
    void        HandleStartAck(unsigned char *data);                       // opcode 0x06
	void	    HandleTurn(unsigned char *data);           				// opcode 0x12
	void    	HandleTilt(unsigned char *data);	            			// opcode 0x13
    void        HandleMove(unsigned char *data);                           // opcode 0x14
    void        HandleRequestTarget(unsigned char *data);                  // opcode 0x17
	void	    HandleRequestTargetsTarget(unsigned char *data);			// opcode 0x18
    void        HandleDebug(unsigned char *data);                          // opcode 0x1A
	void	    HandleInventoryMove(unsigned char *data);					// opcode 0x27 
	void	    HandleItemState(unsigned char *data);                      // opcode 0x29
    void        HandleAction(unsigned char *data);							// opcode 0x2C
    void        HandleOption(unsigned char *data);                         // opcode 0x2E
    void        HandleClientChat(unsigned char *data);                     // opcode 0x33
	void		HandleSlashCommands(char *Msg);
    void        HandleMasterJoin(unsigned char *data);                     // opcode 0x35
    void        HandleRequestTime(unsigned char *data);                    // opcode 0x44
    void        HandleStarbaseRequest(unsigned char *data);                // opcode 0x4E
	void	    HandleSkillStringRequest(unsigned char *data);				// opcode 0x51
    void        HandleSelectTalkTree(unsigned char *data);                 // opcode 0x55
	void	    HandleEquipUse(unsigned char *data);						// opcode 0x5D
    void        HandleVerbRequest(unsigned char *data);                    // opcode 0x5A
	void        HandleChatStream(unsigned char *data);						// opcode 0x5E
    void        HandleGlobalConnect(unsigned char *data);                  // opcode 0x6D
    void        HandleGlobalTicketRequest(unsigned char *data);            // opcode 0x6E
    void        HandleDeleteCharacter(unsigned char *data);                // opcode 0x71
    void        HandleCreateCharacter(unsigned char *data);                // opcode 0x72
	void	    HandleMissionForfeit(unsigned char *data);					// opcode 0x86
	void	    HandleMissionDismissal(unsigned char *data);				// opcode 0x87
    void        HandlePetitionStuck(unsigned char *data);                  // opcode 0x88
    void        HandleIncapacitanceRequest(unsigned char *data);           // opcode 0x8D
    void        HandleGalaxyMapRequest();                                  // opcode 0x98
	void	    HandleWarp(unsigned char *data);							// opcode 0x9B
    void        HandleStarbaseAvatarChange(unsigned char *data);           // opcode 0x9D
    void        HandleStarbaseRoomChange(unsigned char *data);             // opcode 0x9F
    void        HandleTriggerEmote(unsigned char *data);                   // opcode 0xA1
    void        HandleClientChatRequest(unsigned char *data);              // opcode 0xA3
    void        HandleLogoffRequest(unsigned char *data);                  // opcode 0xB9
	void	    HandleCTARequest(unsigned char *data);						// opcode 0xBC
    void	    HandleActionResponse(unsigned char *data);
    void        Contrails(long player_id, bool contrails);

    void        SendLogoffConfirmation();               // opcode 0xba
    void        SendTalkTreeAction(long action);        // opcode 0x56

    void        ActivateProspectBeam(long player_id, long target_id, char *message, short effect_type, long effectUID, long timestamp, short effect_time);

    bool        HandleMobCreateRequest(char *param);
    bool	    HandleObjCreateRequest(char *param);
    bool	    HandleFetchRequest();
    bool	    HandleFaceRequest(long Target);
	bool		HandleFaceMeRequest(long Target);
	bool		HandlePanRequest(char *param, int axis); 
	bool		HandleRotateRequest(char *param, int axis);
	bool		HandleLevelOutRequest(long Target);
	bool		HandleCommitRequest(long Target);
	bool		HandleChangeFieldRequest(char *param, int option);
	bool	    HandleGotoRequest();
    bool        HandleObjectHijack();
    bool	    HandleKick(char *param);
    bool	    HandleMoveRequest(char *param);
    bool        HandleOrientationRequest(char *param);
    bool	    HandleEulerOrientationRequest(char *orientation);	// used by '/euler' command
    bool	    HandleRenderStateRequest();
    void        HandleReleaseHijack();
	bool	    HandleRenderStateInitRequest(char *render_state);
	bool	    HandleRenderStateActivateRequest(char *param);
	bool	    HandleRenderStateActivateNextRequest(char * param);
	bool	    HandleRenderStateDeactivate();
	void	    HandleSendVerbRequest(char *param);
    bool	    HandleRangeRequest();
    bool        HandleScaleRequest(char *param);
	bool		HandleSpinRequest(char *param);
	bool		HandleNavChangeRequest(char *param, int option);
	bool		HandleTiltRequest(char *param);
	bool		HandleBassetRequest(char *param);
    bool	    HandleWormholeRequest(char *sector);	// used by '/wormhole' command
	bool		HandleAggroSetting(char *param);
	bool		HandleBaseItemListCreate();

    void	    ProcessConfirmedActionOffer();

    long        ToBufferEnd(long cache_index)                                   { return (CIRCULAR_CACHE_SIZE - cache_index); }
    long        BuildCachePacket();
    long        BuildCachePacket(long index);
    short       ReadBuffer(u8 *buff, long &read_ptr_index);
    void        ReSendOpcodes(unsigned char *data);                         // opcode 0x2017

	//helper hacks for devs
	void		SetOverrideFaction(bool f_override)								{ m_Faction_Override = f_override; }

public:

	void		SendConfirmation(char * msg, int PlayerID, int Ability);
    void        SendResourceName(long resourceID, char *resource_name);
    void        SendHuskContent(Object *husk);
    void        SendHuskName(Object *husk);
    void        SendMobName(Object *mob);
    void        SendSimpleAuxName(Object *obj);
    void        SendAuxNameSignature(Object *obj);
    void        SendAuxNameResource(Object *obj);

    void        UnSetTarget(long GameID);
    void        SendOpcode(short opcode, unsigned char *data = (0), long length = 0, bool issue = false);
    void        SendStarbaseSet(char action, char exit_mode);
    void        SendClientSetTime(long TimeSent);
    void	    SendConfirmedActionOffer();
    void        SendObjectToObjectLinkedEffect(Object *target, Object *source, short effect1, short effect2, float speedup = 1.0f);
    void        SendClientSound(char *sound_name, long channel = 0, char queue = 0);
    void        PointEffect(float *position, short effect_id, float scale = 1.0f);
    void        SendObjectEffect(ObjectEffect *object_effect);
    void        SendClientType(long client_type);       // opcode 0x3c
    void        SendStart(long start_id);               // opcode 0x05
    void        SendSetBBox(float xmin, float ymin, float xmax, float ymax);
    void        SendSetZBand(float min, float max);     // opcode 0x2a
    void        SendNavigation(int game_id, float signature, char visited, int nav_type, char is_huge);
    void        SendCreate(int game_id, float scale, short asset, int type, float h=0.0, float s=0.0, float v=0.0);
    void        SendDecal(int game_id, int decal_id, int decal_count);
    void        SendNameDecal(int game_id, char *shipname, float h=1.0, float s=1.0, float v=1.0);
    void        SendConstantPositionalUpdate(long game_id, float x, float y, float z, float *orientation=NULL);
    void        SendFormationPositionalUpdate(long leader_id, long target_id, float x, float y, float z);
    void        SendSimplePositionalUpdate(long object_id, PositionInformation * position_info);
    void        SendPlanetPositionalUpdate(long object_id, PositionInformation * position_info);
    void        SendComponentPositionalUpdate(long object_id, PositionInformation * position_info, long timestamp=0);
    void        SendAdvancedPositionalUpdate(long object_id, PositionInformation * position_info);
    void        SendObjectToObjectEffect(ObjectToObjectEffect *obj_effect);
    void        SendActivateRenderState(long game_id, unsigned long render_state_id);
    void        SendInitRenderState(long game_id, unsigned long render_state_id);
    void        SendActivateNextRenderState(long game_id, unsigned long render_state_id);
    void        SendDeactivateRenderState(long game_id);
    void        OpenInterface(long UIChange, long UIType);
    void        SendSetTarget(int game_id, int target_id);
    void        SendPushMessage(char *msg1, char *type, long time, long priority);
    void        SetResourceDrainLevel(Object *obj, long slot);
	void		SetHuskDrainLevel(Object *obj, long slot);
    void        RemoveObject(long object_id);
    void        SendProspectAUX(long value, int type); //HAX! Remove when decyphered
    void        CreateTractorComponent(float *position, float decay, float tractor_speed, long player_id, long article_id, long effect_id, long timestamp);
    bool        CheckResourceLock(long object_id);
    long        CurrentResourceTarget();
    void        CloseInterfaceIfTargetted(long target_id);
    void        CloseInterfaceIfOpen();
    void        SendAttackerUpdates(long mob_id, long update);
    void        SendChangeBasset(ChangeBaseAsset *NewAsset);
    void        CheckObjectRanges();
    void        SendRemoveEffect(int target_id);
    void        TradeAction(long GameID, int Action);
    void        SendResourceContentsAUX(Object *obj);
    void        CancelTrade();
	long		CalculateTradeStack(_Item* item);
    void        SendResourceLevel(long target_id);
    void        SendCameraControl(long Message, long GameID);
    void        SendClientDamage(long target_id, long source_id, float damage, float modifier, long type, long inflicted = 0);
    void        Dialog(char *Stringd, int Type);
    void        ForceLogout();
    void        ChangeSectorID(long SectorID);
    void        OpenStargate(long object_id);
    void        CloseStargate(long object_id);
    bool        SendLoungeNPC(long StationID);
    void        SetManufactureID(long mfg_id);          // opcode 0x7f
    void        SendServerHandoff(long from_sector_id, long to_sector_id, char *from_sector, char *from_system, char *to_sector, char *to_system);
    void        SendNotifyEmote(long game_id, long emote);
    void        SendClientChatEvent(int Unknown, int Unknown2, char * FName, char * LName, char * Channel, char * Message);
    void        SendRelationship(long ObjectID, long Reaction, bool IsAttacking);
    void        SendGalaxyMap(char *system, char *sector, char *station);
    void        SetStartingPosition();
    void        SendPriorityMessageString(char *msg1, char *msg2, long time, long priority);
    void        SendWarpIndex(int index);
    void        SendCreateAttachment(int parent, int child, int slot);

    bool        MatchOptWithParam (char *option, char *arg, char *&param, bool &msg_sent);

    long        TryLoungeFile(long sector_id);
    void        SendDataFileToClient(char *filename, long avatar_id=0);
    void        SendDataFileToClientTCP(char *filename, long avatar_id=0);

	void		SetConfirmation(int x)						{ m_Confirmation = x; }
	bool		ConfirmationBusy()							{ return (m_Confirmation > 0); }
    void        WaitForAuxResponse();
    void        SetLoungeReady()                            { m_AuxWaiting = false; }
    void        SetTCPLoginLink(Connection *connection)     { m_LoginConnection = connection; }
    Connection *TCPLoginLink()                              { return m_LoginConnection; }
    void        SetNavCommence()                            { m_NavCommence = true; }
    bool        WaitForNavCommence();
	long		GetSectorNextObjID();
	SectorManager *GetSectorManager();

	bool		GetStartStatus()							{ return m_SentStart; }
	bool		GetOverrideFaction()						{ return m_Faction_Override; }
	bool		GetProspectWindowOpen()						{ return m_ProspectWindow; }

private:	
	TalkTree        * m_CurrentTalkTree;		// Holds parsed data for talk tree
	StationTemplate * m_CurrentStation;			// Holds the Current Station data
	NPCTemplate		* m_CurrentNPC;				// Holds the current NPC Data

	AbilityBase		* m_AbilityList[150];		// Ability List

	int m_BaseID[100];							// Base Stat ID's


	// Conformation dialog
	int m_Confirmation;					// 0 = Not Busy 1 = Group 2 = Confromation
	int m_Confirmation_PlayerID;
	int m_Confirmation_Ability;

	float m_DamageAbsorb;		// Damage absorb

    long m_GroupID;
    long m_CharacterID;    //this is the number used for account management (avatar_id)
    long m_CharacterSlot;  //this is the character's slot number in a user's account

    Group           * m_Group;
	bool              m_AcceptedGroupInvite;

	AuxPlayerIndex	  m_PlayerIndex;
	AuxShipIndex	  m_ShipIndex;
    AuxManufacturingIndex m_ManuIndex;
    CharacterDatabase m_Database;

    // shield and reactor
	float             m_ReactorChargeRate;
	float             m_ShieldChargeRate;
	unsigned long     m_LastReactorChange;
	unsigned long     m_LastShieldChange;

    //manufacturing
    unsigned int      m_NumFormulas;
    bool              m_Manufacturing;

	bool			  m_SentStart;

    //Misc
    bool              m_Remove; //setting this will remove the player at the next safe opportunity.
    bool              m_DebugPlayer;
    bool              m_FirstLogin;
    char            * m_AccountUsername;
    short             m_SetUpdate;
    bool              m_InSpace;
    bool              m_Prospecting;
	bool			  m_Looting;
    Player          * m_MyDebugPlayer;
    ItemList          m_ItemList;
    Object          * m_NearestNav;
    VerbSlot          m_Verbs[4];
    long              m_LoadFlags;
    short             m_AttacksThisTick;
    bool              m_LogDockCoords;
	bool			  m_SentDockPos;	//set this so we only send one position update while a player is docking for neatness
	bool			  m_SendDockPos;
	bool			  m_Faction_Override;
    MasterJoin        m_MasterJoin;
    Connection      * m_LoginConnection;
	long			  m_RegisteredSectorID;
	long			  m_WeaponsPerTick; //this keeps track of how many weapons FX have been sent per tick (100ms)
	u32				  m_BroadcastPositionTick; //used to split up the starbase broadcasts
	u16				  m_OpcodeResends;
	u8				  m_LoginStage;
	int				  m_StealthLevel;
	AbilityBase		* m_CurrentSkill;
	u32				  m_GameIndex;

    float             m_DockCoords[3];
    float             m_DockHeading[3];
	u8				  m_PDAFactionID[32]; //keep track of PDA order vs faction ID
	bool			  m_ChannelSubscription[32];

    //range lists and object ranging indicies
    long              m_ObjRangeList[MAX_OBJS_PER_SECTOR/32]; //array of objects which are currently marked as in range
    long              m_ResourceSendList[MAX_OBJS_PER_SECTOR/32];

	long			  m_NavsExposed[MAX_NUMBER_OF_GALAXY_OBJECTS/32];
	long			  m_NavsExplored[MAX_NUMBER_OF_GALAXY_OBJECTS/32];
    bool              m_FoundAllSectorNavs[MAX_NUMBER_OF_SPACE_SECTORS];

    u32				  m_RangeList[MAX_ONLINE_PLAYERS/32 + 1]; // list of Players who can see this player
	u32				  m_ClickedList[MAX_ONLINE_PLAYERS/32 + 1];// list of Players who have clicked on this player

	long			  m_NavRange_1[MAX_NAVS_DECOS/32];
	long			  m_NavRange_2[MAX_NAVS_DECOS/32];
	long			  m_NavRange_3[MAX_NAVS_DECOS/32];
	bool			  m_Arrival_Flag;

	//ship slots
	u8				  m_WeaponSlots;
	u8				  m_DeviceSlots;

    //movement on stations
    bool              m_HavePosition;
    long              m_Room;
    long	          m_Oldroom;
    long              m_ActionFlag;
    float             m_Orient;

    //movement in space
    bool              m_FollowObject;

    //MVAS members
    bool              m_ReceivedMVAS;
    float             m_LastPos[3];
    unsigned long     m_LastRead;
    long              m_MVAS_index; //if -1 then use Server forced positioning, rather than client based positioning
    u16				  m_MVAS_frequency;
    float             m_LastVelocity;
    unsigned char     m_UDPSendBuffer[UDP_BUFFER_SEND_SIZE];

    //warp data
  	long			  m_WarpNavs[20];
	long			  m_WarpNavIndex;
	long			  m_WarpEffect;
    bool              m_WarpDrive;
    float             m_DistToNav;
    short             m_OverrunCount;
    short             m_WarpNavCount;
    unsigned long     m_WarpTime;
    unsigned long     m_WarpBroadcastTime;

    //energy
    unsigned long     m_EnergyRecharge;
    unsigned long     m_ShieldRecharge;

    //start sector methods
    long              m_CameraSignal;  //if this is anything other than -1, we send a camera signal at sector login
    long              m_CameraID;

    long              m_FromSectorID;     // how did we get here?

    //Connection members
    UDP_Connection  * m_UDPConnection;
    MessageQueue    * m_RecvQueue;
    long              m_PacketSequenceNum;
    long              m_Player_IPAddr;
    short             m_Player_Port;
    bool              m_UDPHandoffReceived;
    bool              m_Connection_Available;

	unsigned long	  m_QueueCacheIndex;

	unsigned char	  m_QueueCache[MAXIMUM_PACKET_CACHE];
    unsigned char     m_ScratchBuffer[MAXIMUM_PACKET_CACHE];   //used for general scratch data, packet forming and sending data to player via UDP
	MessageQueue	* m_SendQueue;

	MessageQueue	* m_SendQueueBk;  //helper to trace problem

	//talk tree handling
	char			  m_TalkTreeBuffer[TALKTREE_BUFFER_SIZE];

    //manufacturing
    ItemBase        * m_CurManuItem;

    //combat
    bool              m_AttackMusic;
    short             m_AttackerCount;
	bool			  m_IncapAvatarSent;
	bool			  m_CombatImmunity;


    //prospecting & loot
    bool		      m_TractorBeam;
    bool		      m_ProspectBeam;
	TimeNode          m_ProspectBeamNode;
	TimeNode          m_ProspectTractorNode;
    float             m_TractorSpeed;
	ItemBase	    * m_FloatingOre_contents;
	short             m_FloatingOre_stack;

    //hijack
    long			  m_Hijackee;

    //missions
    unsigned long     m_MissionFlags;
    bool              m_MissionAcceptance;

    bool              m_ActionResponseReceived;
    bool		      m_ProspectWindow;
    long			  m_TradeID;			// ID of person you are tradeing with
    bool              m_TradeConfirm;
    Object          * m_CurrentDecoObj; //this is used by content devs creating decos
    bool		      m_TradeWindow;
    long              m_StarbaseTargetID;
	long			  m_PushMissionID;
	long			  m_PushMissionUID;
    bool			  m_BeaconRequest;

    bool              m_AuxWaiting;
    bool              m_NavCommence;
    bool              m_LoginFailed;
	bool			  m_ExposeDecos;
    long              m_StargateDestination;
	long			  m_LastMissionCompletion;

public:
    //TODO: these need to be encapsulated & made private
    Equipable         m_Equip[20];
	Stats			  m_Stats;
	Effects			  m_Effects;
	PlayerBuffs		  m_Buffs;
};

#define ABILITY_JUMPSTART	65

#endif // _PLAYER_CLASS_H_INCLUDED_
