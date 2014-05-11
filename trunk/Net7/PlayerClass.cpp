// PlayerClass.cpp

#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"
#include "Opcodes.h"
#include "PacketMethods.h"
#include "StaticData.h"

// Load in all Abilities!
#include "Abilities.h"

//#define MOVEMENT_DEBUG

Player::Player(long object_id) : Object (object_id)
{
    m_Type = OT_PLAYER;
    m_Remove = false;
	m_RecvQueue = new MessageQueue;
	m_SendQueue = new MessageQueue(g_ServerMgr->GetUDPCBuffer());
	m_SendQueueBk = m_SendQueue;

	SetupAbilities();
}

Player::Player(void) : Object (-1) //default constructor so we can create an array of Player
{
    m_Type = OT_PLAYER;
    m_Remove = false;
	m_RecvQueue = new MessageQueue;
	m_SendQueue = new MessageQueue(g_ServerMgr->GetUDPCBuffer());
	m_SendQueueBk = m_SendQueue;

	SetupAbilities();
}

Player::~Player()
{
    // TODO: destroy everything
}

/*
* //TO-DO: Add all skills in here.
*/
void Player::SetupAbilities()
{
	// Reset data
	memset(m_AbilityList, 0, sizeof(m_AbilityList));

	m_CurrentSkill = NULL;

	// Ability List
	/*
	* This may not seem intuative, but what it does is link all SkillRanks into
	*  the array so that they all point to a single copy of the class that handles
	*  them. The reason for doing this is so that we can have the access time of an array
	*  for calling a skill class without having to search through each class to determine
	*  if the class contains the SkillRank we need.
	*
	* For sanity's sake, the class is declared with the name of the lowest-rank skill
	*   for a given ability (EX: For Recharge Shields, the 1st rank of that ability is
	*   called RegerenateShields, so that class is created, and all further ranks of the
	*   skill point to that class.
	*
	* Also, abilities are organized Alphabetically for easy finding.
	*/

	//Cloak Ability
	m_AbilityList[CLOAK] = new ACloak(this); //rank 1
	m_AbilityList[ADVANCED_CLOAK] = m_AbilityList[CLOAK]; //rank 3
	m_AbilityList[COMBAT_CLOAK] = m_AbilityList[CLOAK]; //rank 5
	m_AbilityList[GROUP_STEALTH] = m_AbilityList[CLOAK]; //rank 6
	m_AbilityList[GROUP_CLOAK] = m_AbilityList[CLOAK]; //rank 7

	//Hull Patch ability
	m_AbilityList[PATCH_HULL] = new AHullPatch(this); //rank 1
	m_AbilityList[REPAIR_HULL] = m_AbilityList[PATCH_HULL]; //rank 3
	m_AbilityList[COMBAT_HULL_REPAIR] = m_AbilityList[PATCH_HULL]; //rank 5
	m_AbilityList[AREA_HULL_REPAIR] = m_AbilityList[PATCH_HULL]; //rank 6
	m_AbilityList[IMPROVED_AREA_HULL_REPAIR] = m_AbilityList[PATCH_HULL]; //rank 7

	// Worm Hole Ability
	m_AbilityList[WORMHOLE_ASTEROID_BELT_BETA] = new AWormHole(this);
	m_AbilityList[WORMHOLE_CARPENTER] = m_AbilityList[WORMHOLE_ASTEROID_BELT_BETA];
	m_AbilityList[WORMHOLE_ENDRIAGO] = m_AbilityList[WORMHOLE_ASTEROID_BELT_BETA];
	m_AbilityList[WORMHOLE_JUPITER] = m_AbilityList[WORMHOLE_ASTEROID_BELT_BETA];
	m_AbilityList[WORMHOLE_KAILAASA] = m_AbilityList[WORMHOLE_ASTEROID_BELT_BETA];
	m_AbilityList[WORMHOLE_SWOOPING_EAGLE] = m_AbilityList[WORMHOLE_ASTEROID_BELT_BETA];
	m_AbilityList[WORMHOLE_VALKYRIE_TWINS] = m_AbilityList[WORMHOLE_ASTEROID_BELT_BETA];

	//Jump Start ability
	m_AbilityList[JUMPSTART] = new AJumpStart(this);

	//Recharge Sheilds ability
	m_AbilityList[REGENERATE_SHIELDS] = new ARechargeShields(this); //rank 1
	m_AbilityList[RECHARGE_SHIELDS] = m_AbilityList[REGENERATE_SHIELDS]; //rank 3
	m_AbilityList[COMBAT_RECHARGE_SHIELDS] = m_AbilityList[REGENERATE_SHIELDS]; //rank 5
	m_AbilityList[AREA_SHIELD_RECHARGE] = m_AbilityList[REGENERATE_SHIELDS]; //rank 6
	m_AbilityList[IMPROVED_AREA_RECHARGE] = m_AbilityList[REGENERATE_SHIELDS]; //rank 7

	//Shield Sap ability, EffectID: 146
	m_AbilityList[SHIELD_SAP] = new AShieldSap(this); //rank 1
	m_AbilityList[SHIELD_TRANSFER] = m_AbilityList[SHIELD_SAP]; //rank 3
	m_AbilityList[GROUP_SAP] = m_AbilityList[SHIELD_SAP]; //rank 5
	m_AbilityList[SAPPING_SPHERE] = m_AbilityList[SHIELD_SAP]; //rank 6
	m_AbilityList[GROUP_SAPPING_SPHERE] = m_AbilityList[SHIELD_SAP]; //rank 7

	/* These skills are not yet implemented
	//Repair Equipement ability, EffectID: 178
	m_AbilityList[REGENERATE_EQUIPMENT] = new ARepairEquipment(this) //rank 1
	m_AbilityList[REPAIR_EQUIPMENT] = m_AbilityList[REGENERATE_EQUIPMENT]; //rank 3
	m_AbilityList[COMBAT_EQUIPMENT_REPAIR] = m_AbilityList[REGENERATE_EQUIPMENT]; //rank 5
	m_AbilityList[AREA_EQUIPMENT_REPAIR] = m_AbilityList[REGENERATE_EQUIPMENT]; //rank 6
	m_AbilityList[IMPROVED_AREA_REPAIR] = m_AbilityList[REGENERATE_EQUIPMENT]; //rank 7

	//Shield Charging ability
	m_AbilityList[SUPERCHARGE_SHIELDS] = new AShieldCharge(this); //rank 1
	m_AbilityList[ULTRACHARGE_SHIELDS] = m_AbilityList[SUPERCHARGE_SHIELDS]; //rank 3
	m_AbilityList[SUPERCHARGE_TARGET] = m_AbilityList[SUPERCHARGE_SHIELDS]; //rank 5
	m_AbilityList[ULTRACHARGE_TARGET] = m_AbilityList[SUPERCHARGE_SHIELDS]; //rank 6
	m_AbilityList[MEGACHARGE_SHIELDS] = m_AbilityList[SUPERCHARGE_SHIELDS]; //rank 7

	//Befriend ability, EffectID: 221
	m_AbilityList[BEFRIEND] = new ABefriend(this); //rank 1
	m_AbilityList[IMPROVED_BEFRIEND] = m_AbilityList[BEFRIEND]; //rank 3
	m_AbilityList[ENTRANCE] = m_AbilityList[BEFRIEND]; //rank 5
	m_AbilityList[SOOTHE] = m_AbilityList[BEFRIEND]; //rank 6
	m_AbilityList[AREA_SOOTHE] = m_AbilityList[BEFRIEND]; //rank 7

	//Enrage skill, EffectID: 212
	m_AbilityList[ANGER] = new AEnrage(this); //rank 1
	m_AbilityList[CAUSE_AGGRESSION] = m_AbilityList[ANGER]; //rank 3
	m_AbilityList[ENRAGE] = m_AbilityList[ANGER]; //rank 5
	m_AbilityList[ANGER_GROUP] = m_AbilityList[ANGER]; //rank 6
	m_AbilityList[ENRAGE_GROUP] = m_AbilityList[ANGER]; //rank 7

	//Gravity Link skill, EffectID: 219
	m_AbilityList[MASS_FIELD] = new AGravityLink(this); //rank 1
	m_AbilityList[GRAVITY_FIELD] = m_AbilityList[MASS_FIELD]; //rank 3
	m_AbilityList[IMMOBILIZATION_FIELD] = m_AbilityList[MASS_FIELD]; //rank 5
	m_AbilityList[AREA_MASS_FIELD] = m_AbilityList[MASS_FIELD]; //rank 6
	m_AbilityList[AREA_IMMOBILIZATION_FIELD] = m_AbilityList[MASS_FIELD]; //rank 7

	//Self-Destruct Skill, EffectID: 206
	m_AbilityList[SELF_DESTRUCT_1] = new ASelfDestruct(this); //rank 1
	m_AbilityList[SELF_DESTRUCT_2] = m_AbilityList[SELF_DESTRUCT_1]; //rank 3
	m_AbilityList[SELF_DESTRUCT_3] = m_AbilityList[SELF_DESTRUCT_1]; //rank 5
	m_AbilityList[SELF_DESTRUCT_4] = m_AbilityList[SELF_DESTRUCT_1]; //rank 6
	m_AbilityList[SELF_DESTRUCT_5] = m_AbilityList[SELF_DESTRUCT_1]; //rank 7

	//Shield Nova skill, EffectID: 98
	m_AbilityList[SHIELD_RAM] = new AShieldInversion(this); //rank 1 
	m_AbilityList[SHIELD_SPIKE] = m_AbilityList[SHIELD_RAM]; //rank 3
	m_AbilityList[SHIELD_BURN] = m_AbilityList[SHIELD_RAM]; //rank 5
	m_AbilityList[SHIELD_FLARE] = m_AbilityList[SHIELD_RAM]; //rank 6
	m_AbilityList[SHIELD_NOVA] = m_AbilityList[SHIELD_RAM]; //rank 7

	//Hacking skill, , EffectID (see effect.ini for all effect-ids): 193
	m_AbilityList[HACK_SYSTEMS] = new AHacking(this); //rank 1
	m_AbilityList[HACK_WEAPONS] = m_AbilityList[HACK_SYSTEMS]; //rank 3
	m_AbilityList[MULTI_HACK] = m_AbilityList[HACK_SYSTEMS]; //rank 5
	m_AbilityList[AREA_SYSTEM_HACK] = m_AbilityList[HACK_SYSTEMS]; //rank 6
	m_AbilityList[AREA_MULTI_HACK] = m_AbilityList[HACK_SYSTEMS]; //rank 7

	//Biorepression skill
	m_AbilityList[BIOREPRESS] = new ABiorepression(this); //rank 1
	m_AbilityList[BIOSUPPRESS] = m_AbilityList[BIOREPRESS]; //rank 3
	m_AbilityList[BIOREPRESSION_SPHERE] = m_AbilityList[BIOREPRESS]; //rank 5
	m_AbilityList[BIOSUPPRESSION_SPHERE] = m_AbilityList[BIOREPRESS]; //rank 6
	m_AbilityList[BIOCESSATION] = m_AbilityList[BIOREPRESS]; //rank 7

	//Rally skill
	m_AbilityList[DAMAGE_TACTICS] = new ARally(this); //rank 1
	m_AbilityList[DEFENSE_TACTICS] = m_AbilityList[DAMAGE_TACTICS]; //rank 3
	m_AbilityList[FIRING_TACTICS] = m_AbilityList[DAMAGE_TACTICS]; //rank 5
	m_AbilityList[STEALTH_TACTICS] = m_AbilityList[DAMAGE_TACTICS]; //rank 7

	//Compulsory Contemplation skill
	m_AbilityList[COMPULSORY_CONTEMPLATION] = new ACompulsoryContemplation(this); //rank 1

	//Environmental shield skill, EffectID: 216
	m_AbilityList[ENVIRONMENTAL_BARRIER] = new AEnvironmentalShield(this); //rank 1
	m_AbilityList[LESSER_ENVIRONMENTAL_SHIELD] = m_AbilityList[ENVIRONMENTAL_BARRIER]; //rank 3
	m_AbilityList[ENVIRONMENTAL_SHIELD] = m_AbilityList[ENVIRONMENTAL_BARRIER]; //rank 5
	m_AbilityList[GREATER_ENVIRONMENTAL_SHIELD] = m_AbilityList[ENVIRONMENTAL_BARRIER]; //rank 6
	m_AbilityList[ULTRA_ENVIRONMENTAL_SHIELD] = m_AbilityList[ENVIRONMENTAL_BARRIER]; //rank 7

	//Fold Space skill, EffectID: 202
	m_AbilityList[TELEPORT_SELF] = new AFoldSpace(this); //rank 1
	m_AbilityList[TELEPORT_ENEMY] = m_AbilityList[TELEPORT_SELF]; //rank 3
	m_AbilityList[TELEPORT_FRIEND] = m_AbilityList[TELEPORT_SELF]; //rank 5
	m_AbilityList[DIRECTIONAL_TELEPORT] = m_AbilityList[TELEPORT_SELF]; //rank 5
	m_AbilityList[AREA_TELEPORT] = m_AbilityList[TELEPORT_SELF]; //rank 7

	//Sheild Leech skill
	m_AbilityList[SHIELD_DRAIN] = new AShieldLeech(this); //rank 1
	m_AbilityList[SHIELD_LEECH] = m_AbilityList[SHIELD_DRAIN]; //rank 3
	m_AbilityList[GROUP_LEECH] = m_AbilityList[SHIELD_DRAIN]; //rank 5
	m_AbilityList[SHIELD_LEECHING_SPHERE] = m_AbilityList[SHIELD_DRAIN]; //rank 6
	m_AbilityList[GROUP_LEECHING_SPHERE] = m_AbilityList[SHIELD_DRAIN]; //rank 7

	//Repulsor Field Skill
	m_AbilityList[MINOR_REPULSOR_FIELD] = new ARepulsorField(this); //rank 1
	m_AbilityList[LESSER_REPULSOR_FIELD] = m_AbilityList[MINOR_REPULSOR_FIELD]; //rank 3
	m_AbilityList[REPULSOR_FIELD] = m_AbilityList[MINOR_REPULSOR_FIELD]; //rank 5
	m_AbilityList[GREATER_REPULSOR_FIELD] = m_AbilityList[MINOR_REPULSOR_FIELD]; //rank 6
	m_AbilityList[MAJOR_REPULSOR_FIELD] = m_AbilityList[MINOR_REPULSOR_FIELD]; //rank 7

	//Power Down skill
	m_AbilityList[POWER_DOWN] = new APowerDown(this); //rank 1
	m_AbilityList[ADVANCED_POWER_DOWN] = m_AbilityList[POWER_DOWN]; //rank 3
	m_AbilityList[ADVANCED_POWER_DOWN_2] = m_AbilityList[POWER_DOWN]; //rank 5
	m_AbilityList[ADVANCED_POWER_DOWN_3] = m_AbilityList[POWER_DOWN]; //rank 6
	m_AbilityList[ADVANCED_POWER_DOWN_4] = m_AbilityList[POWER_DOWN]; //rank 7

	//Menace skill, EffectID (scare): 199, , EffectID (intimidate): 198
	m_AbilityList[INTIMIDATE] = new AMenace(this); //rank 1
	m_AbilityList[SCARE] = m_AbilityList[INTIMIDATE]; //rank 3
	m_AbilityList[TERRIFY] = m_AbilityList[INTIMIDATE]; //rank 5
	m_AbilityList[AREA_INTIMIDATE] = m_AbilityList[INTIMIDATE]; //rank 6
	m_AbilityList[AREA_TERRIFY] = m_AbilityList[INTIMIDATE]; //rank 7

	//Energy Leech skill
	m_AbilityList[ENERGY_DRAIN] = new AEnergyLeech(this); //rank 1
	m_AbilityList[ENERGY_LEECH] = m_AbilityList[ENERGY_DRAIN]; //rank 3
	m_AbilityList[RENDER_ENERGY] = m_AbilityList[ENERGY_DRAIN]; //rank 5
	m_AbilityList[ENERGY_LEECHING_SPHERE] = m_AbilityList[ENERGY_DRAIN]; //rank 6
	m_AbilityList[RENDER_ENERGY_SPHERE] = m_AbilityList[ENERGY_DRAIN]; //rank 7

	//Psionic Shield skill, EffectID: 214
	m_AbilityList[PSIONIC_BARRIER] = new APsionicShield(this); //rank 1
	m_AbilityList[LESSER_PSIONIC_SHIELD] = m_AbilityList[PSIONIC_BARRIER]; //rank 3
	m_AbilityList[PSIONIC_SHIELD] = m_AbilityList[PSIONIC_BARRIER]; //rank 5
	m_AbilityList[GREATER_PSIONIC_SHIELD] = m_AbilityList[PSIONIC_BARRIER]; //rank 6
	m_AbilityList[PSIONIC_INVULNERABILITY] = m_AbilityList[PSIONIC_BARRIER]; //rank 7

	//Summon skill
	m_AbilityList[SUMMON_ENEMY] = new ASummon(this); //rank 1
	m_AbilityList[SUMMON_FRIEND] = m_AbilityList[SUMMON_ENEMY]; //rank 3
	m_AbilityList[SUMMON_GROUP] = m_AbilityList[SUMMON_ENEMY]; //rank 5
	m_AbilityList[SUMMON_ENEMY_GROUP] = m_AbilityList[SUMMON_ENEMY]; //rank 6
	m_AbilityList[RETURN_FRIEND] = m_AbilityList[SUMMON_ENEMY]; //rank 7
	*/
}

void Player::ResetPlayer()
{
    SetLastAccessTime(0);
	m_DamageAbsorb = 0;
    m_GroupID = -1;
	m_AcceptedGroupInvite = false;
    m_MVAS_index = -1;
    BlankPositionInfo();
    m_Remove = false;
    m_Room = 0;
    m_Oldroom = 0;
    m_LastRead = 0;
    m_ReceivedMVAS = false;
    m_MovementID = 0;
    m_Group = 0;
    m_Active = false;
    m_Accelerating = false;
    m_Velocity = 0;
    m_YInput = 0;
    m_ZInput = 0;
    m_DebugPlayer = false;
    m_Connection_Available = false;
	m_ToBeRemoved = false;
    m_CameraSignal = -1;
    m_AccountUsername = g_StringMgr->NullStr();
    m_FirstLogin = true;
    m_Hijackee = (0);
    m_WarpDrive = false;
    m_DistToNav = 0.0f;
    m_OverrunCount = 0;
    m_NumFormulas = 0;
    m_MVAS_frequency = 10;
    m_FollowObject = false;
    m_Prospecting = false;
	m_Looting = false;
    m_AttackMusic = false;
    m_AttackerCount = 0;
    m_MyDebugPlayer = (0);
    m_MissionFlags = 0;
    m_LoadFlags = 0;
    m_ObjectRadius = 245.0f;
    m_UDPConnection = (0);
    m_FromSectorID = 0;
    m_SectorMgr = (0);
	m_ObjectMgr = (0);
    m_LoginConnection = (0);
    m_LoginFailed = false;
    m_CurrentTalkTree = (0);
	m_CurrentStation = (0);
	m_CurrentNPC = (0);
	m_ExposeDecos = false;
	m_WeaponSlots = 0;
	m_DeviceSlots = 0;
	m_Confirmation = 0;
	m_Confirmation_PlayerID = 0;
	m_Confirmation_Ability = 0;
	m_RegisteredSectorID = 0;
	m_QueueCacheIndex = 0;
	m_WeaponsPerTick = 0;
	m_BroadcastPositionTick = 0;
	m_OpcodeResends = 0;
	m_LoginStage = 0;
	m_LastMissionCompletion = 0;
	m_PushMissionID = 0;
	m_PushMissionUID = 0;
	m_Faction_Override = false;
	m_CombatImmunity = false;
	memset(&m_PDAFactionID, 0, sizeof(m_PDAFactionID));

    m_TractorSpeed = 200.f;
	if (m_SendQueue != m_SendQueueBk) m_SendQueue = m_SendQueueBk;

	m_SendQueue->ResetQueue();

	memset(m_AbilityList, 0, sizeof(m_AbilityList));
    memset(m_Verbs, 0, sizeof(m_Verbs));
    memset(m_NavsExplored, 0, sizeof(m_NavsExplored));
    memset(m_NavsExposed, 0, sizeof(m_NavsExposed));
    memset(m_FoundAllSectorNavs, 0, sizeof(m_FoundAllSectorNavs));
	memset(m_ChannelSubscription, 0, sizeof(m_ChannelSubscription));

    memset(&m_ProspectBeamNode, 0, sizeof(TimeNode)); //erase node contents.
    memset(&m_ProspectTractorNode, 0, sizeof(TimeNode)); //erase node contents.

	memset(&m_NavRange_1, 0, sizeof(m_NavRange_1));
	memset(&m_NavRange_2, 0, sizeof(m_NavRange_2));
	memset(&m_NavRange_3, 0, sizeof(m_NavRange_3));
	m_Arrival_Flag = false;

    m_DockCoords[0] = m_DockCoords[1] = m_DockCoords[2] = 0.0f;
    m_DockHeading[0] = m_DockHeading[1] = m_DockHeading[2] = 0.0f;

	// Setup abilities
	SetupAbilities();

	// Setup Stats
	m_Stats.Init(this);
	m_Effects.Init(this);
	m_Buffs.Init(this);

	// Zero out data
	memset(m_BaseID, 0, sizeof(m_BaseID));

    for (int i=0; i<20; i++)
    {
        m_Equip[i].Init(this, i);
    }

	PlayerIndex()->Reset();
	PlayerIndex()->ClearFlags();
	ShipIndex()->Reset();
	ShipIndex()->ClearFlags();
}

//Do not use this to perform actions for login,
//Use 'SectorLogin()' below
void Player::SectorReset()
{
    m_EnergyRecharge = 0;
    m_ShieldRecharge = 0;
    m_Manufacturing = false;
    BlankPositionInfo();
    m_Room = 0;
    m_Oldroom = 0;
    m_HavePosition = false;
    m_Orient = 0.0f;
    m_LastRead = 0;
    m_MovementID = 5;
    m_WarpDrive = false;
    m_Accelerating = false;
    m_Velocity = 0;
    m_YInput = 0;
    m_ZInput = 0;
    m_SetUpdate = 0;
    m_CameraSignal = 0;
    m_CameraID = 0;
    m_DistToNav = 0.0f;
    m_OverrunCount = 0;
    m_FollowObject = false;
    m_Prospecting = false;
	m_Looting = false;
    m_AttackMusic = false;
    m_AttackerCount = 0;
    m_LastVelocity = 0.0f;
    m_WarpTime = 0;
    m_NearestNav = (0);
	m_FloatingOre_contents = (0);
	m_FloatingOre_stack = 0;
    m_MissionAcceptance = false;
    m_ProspectBeam = false;
    m_TractorBeam = false;
    m_WarpBroadcastTime = 0;
    RemoveProspectNodes();
    m_AttacksThisTick = 0;
    m_LogDockCoords = false;
    m_StargateDestination = 0;
    m_ActionResponseReceived = false;
    m_ProspectWindow = false;
    m_TradeID = -1;
    m_TradeConfirm = false;
    m_CurrentDecoObj = (0);
    m_TradeWindow = false;
    m_StarbaseTargetID = 0;
    m_BeaconRequest = 0;
    m_AuxWaiting = true;
    m_NavCommence = false;
    m_PacketSequenceNum = 0;
    m_CurrentTalkTree = (0);
	m_WeaponsPerTick = 0;
	m_SentDockPos = false;
	m_SendDockPos = false;
	m_IncapAvatarSent = false;
    
	m_QueueCacheIndex = 0;
    memset (m_ScratchBuffer, 0, MAXIMUM_PACKET_CACHE);
	memset (m_QueueCache, 0, MAXIMUM_PACKET_CACHE);

    memset(m_ObjRangeList, 0, MAX_OBJS_PER_SECTOR/8);
    memset(m_ResourceSendList, 0, MAX_OBJS_PER_SECTOR/8);

	memset (&m_RangeList, 0, sizeof(m_RangeList));
	memset (&m_ClickedList, 0, sizeof(m_ClickedList));

	memset(&m_NavRange_1, 0, sizeof(m_NavRange_1));
	memset(&m_NavRange_2, 0, sizeof(m_NavRange_2));
	memset(&m_NavRange_3, 0, sizeof(m_NavRange_3));
	m_Arrival_Flag = false;
}

void Player::FirstLogin()
{
    if (m_FirstLogin) //this only executes the first time it is run (per player).
    {
		int class_index = ClassIndex();

		// Setup base ScanRange, impuse, and signature
		m_BaseID[ID_SCAN_RANGE] = m_Stats.SetStat(STAT_BASE_VALUE, STAT_SCAN_RANGE, (float)BaseScanRange[class_index], "BASE_SHIP_VALUE");
		m_BaseID[ID_IMPULSE] = m_Stats.SetStat(STAT_BASE_VALUE, STAT_IMPULSE, (float)BaseSpeed[class_index], "BASE_SHIP_VALUE");
		m_BaseID[ID_SIGNATURE] = m_Stats.SetStat(STAT_BASE_VALUE, STAT_SIGNATURE, (float)BaseVisableRange[class_index], "BASE_SHIP_VALUE");
		m_BaseID[ID_WARP_RECOVERY] = m_Stats.SetStat(STAT_BASE_VALUE, STAT_WARP_RECOVERY, 4000.0f, "BASE_SHIP_VALUE");

		// Update Base Stats

		//go with these for slightly different warp effects...
		long level = 3 + (PlayerIndex()->RPGInfo.GetHullUpgradeLevel() / 2);
		ShipIndex()->BaseStats.SetWarpPowerLevel(level);
		ShipIndex()->CurrentStats.SetWarpPowerLevel(level);

		ShipIndex()->BaseStats.SetScanRange((s32) m_Stats.GetStat(STAT_SCAN_RANGE));
		ShipIndex()->CurrentStats.SetScanRange((s32) m_Stats.GetStat(STAT_SCAN_RANGE));

		ShipIndex()->BaseStats.SetSpeed((s32) m_Stats.GetStat(STAT_IMPULSE));
		ShipIndex()->CurrentStats.SetSpeed((s32) m_Stats.GetStat(STAT_IMPULSE));

		ShipIndex()->BaseStats.SetVisibility((s32) m_Stats.GetStat(STAT_SIGNATURE));
		ShipIndex()->CurrentStats.SetVisibility((s32) m_Stats.GetStat(STAT_SIGNATURE));

		// ------

        ReadSavedData();
		m_FirstLogin = false;

		// Update all skill avalabilitys
		SkillsList();

        //This is temporary and sohuld be removed in a few weeks
        for(int i=0;i<20;i++)
        {
			m_Equip[i].Init(this, i);
			m_Equip[i].PullAuxData();
            if (ShipIndex()->Inventory.AmmoInv.Item[i].GetItemTemplateID() == -2)
            {
                ShipIndex()->Inventory.AmmoInv.Item[i].SetItemTemplateID(-1);
            }
        }

		// Make sure we calculate the quality for every item (should be fixed soon!)
		for(int i=0;i<40;i++)
		{
			QualityCalculator(ShipIndex()->Inventory.CargoInv.Item[i].GetData());
		}
		for(int i=0;i<96;i++)
		{
			QualityCalculator(PlayerIndex()->SecureInv.Item[i].GetData());
		}

        char message[80];
        sprintf(message, "%s has logged in", Name());
        g_PlayerMgr->ChatSendEveryone(GameID(), message, false);
        SendMOTD();
		//Do not send Aux here, it causes PDA corruption
    }
}	

void Player::SetCharacterID(long char_id)
{
    m_CharacterID = char_id;
    //now execute a check to see if this character ID is currently logged in
    if (char_id > 0)
    {
        g_PlayerMgr->CheckForDuplicatePlayers(this);
    }
}

//This gets called every start of sector for players
void Player::SectorLogin()
{
    /* Clear the itembase-sent list */
    m_ItemList.ClearList();

	SetInSpace(false);

    AddPlayerToRangeList(this); //add ourselves to the range list - we're always in range of ourselves

    FirstLogin();


	long new_sector_id = PlayerIndex()->GetSectorNum();

	if (GroupID() > 0)
	{
		g_ServerMgr->m_PlayerMgr.RequestGroupAux(GroupID(),GameID());
	}

	UpdateDatabase();

    if (new_sector_id > 9999) // player is resurrected upon entering starbase - TODO: move to separate method, cost of repair, XP debt
    {
        if (ShipIndex()->GetIsIncapacitated())
        {
            ShipIndex()->SetIsIncapacitated(false);   
			SetDistressBeacon(false);
            ShipIndex()->SetHullPoints(ShipIndex()->GetMaxHullPoints());
            RemobilisePlayer();
        }

		// Find our Station and save the location for this
		// This will make sure we dont have to search again
		m_CurrentStation = g_ServerMgr->m_StationMgr.GetStation(new_sector_id);

		// Save and null out any data
		m_CurrentTalkTree = NULL;
		m_CurrentNPC = NULL;
    }

    if (ShipIndex()->GetIsIncapacitated())
    {
        ImmobilisePlayer();
        SendAuxShip();
    }

    if (m_LoginFailed == true)
    {
        m_LoginFailed = false;
		ObjectManager *om = GetObjectManager();
		if (om) om->RemovePlayerFromMOBRangeLists(this);
    }
}

//this is called when we receive the HandleStartAck from the client - indicates we're in space (99% sure).
void Player::SendLoginCamera()
{
    Object *came_from;
    if (Active())
    {
        ObjectManager *obj_manager = GetObjectManager();
        if (obj_manager) came_from = obj_manager->FindGate(m_FromSectorID);
        if (came_from) //TODO: make a sequence where player spills from opening gate
        {
            SendActivateRenderState(came_from->GameID(), 1); //gate graphics activate
            CloseStargate(came_from->GameID());
        }
        else
        {
            //_sleep(50); //a little time required when you leave stations, but not for arriving from a gate.
        }
        
        SendCameraControl(m_CameraSignal, m_CameraID);
    }
    //This is called once the AckStart is sent
}

//DIMA: Shouldn't this be in playermanager??
//TB: It should be changed so the message is read into heap at startup (startup of PM is ok).
//Then, we should use a 'PushMessage' similar to how MVAS used to eject you, from this class.
//I rekon, anyway. I just hacked it in here so it still works.
//Message of the day.
void Player::SendMOTD()
{
#define MAX_MESSAGE_SIZE 512
    char message[MAX_MESSAGE_SIZE]; //use stack instead of heap.
	// Send MOTD
	FILE *f = fopen("motd.txt", "r");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		long file_size = ftell(f);
		fseek(f, 0, SEEK_SET);
        if (file_size > MAX_MESSAGE_SIZE - 1)
        {
            file_size = MAX_MESSAGE_SIZE - 1;
        }

		long size = fread(message, 1, file_size, f);
		message[size] = 0;

		if (message)
		{
			//SendMessageString("Message of the day:", 3, false);
			char * line = strtok(message, "\n\r");
			do
			{
				SendVaMessage("MOTD: %s", line);
				//SendMessageString(line, 3, false);
				line = strtok(NULL, "\n\r");
			} 
            while (line != 0);
		}

		fclose(f);
	}
}

void Player::SetAccountUsername(char *name)
{
    m_AccountUsername = g_StringMgr->GetStr(name);
}

void Player::SetDatabase(CharacterDatabase &database)
{
    m_Database = database;
    SetName(m_Database.avatar.avatar_first_name);
}

void Player::HandleStarbaseAvatarChange(unsigned char* data)
{
    StarbaseAvatarChange *change = (StarbaseAvatarChange *) data;
    long sector_id = PlayerIndex()->GetSectorNum();

    if (change)
    {      
        Player * p = g_PlayerMgr->GetPlayer(change->AvatarID);

        if (p == (0))
        {
            return;
        }

        if (!m_SectorMgr) return;

		if (p != this)
		{
			Sleep(0); //testing to make sure we only ever see this player
		}

        m_Mutex.Lock();
        if (sector_id != p->m_PlayerIndex.GetSectorNum())
        {
            p->m_PlayerIndex.SetSectorNum(sector_id);
        }
        p->SetPosition(change->Position);
        p->m_Orient = change->Orient;
        p->SetActionFlag(change->ActionFlag);
        p->SetHavePosition();
        m_Mutex.Unlock();
       
        // Broadcast this position to all other players in the same room
        if (p->ActionFlag() == 0x41)			// Send Avatar to everyone
        {
            p->SendStarbaseAvatarList();
			m_BroadcastPositionTick = GetNet7TickCount() + 500; //broadcast position in 0.5 seconds
        }
        else
        {   
            p->BroadcastPosition();
        }
    }
}

void Player::HandleStarbaseRoomChange(unsigned char *data)
{
    StarbaseRoomChange *change = (StarbaseRoomChange *) data;

    Player *p = (0);
    u32 * sector_list = GetSectorPlayerList();

    m_Mutex.Lock();
    m_Oldroom = m_Room;
    m_Room = change->NewRoom;
    SetActionFlag(0x01);
    m_Mutex.Unlock();

    StarbaseRoomChange SRoomUpdate;
    SRoomUpdate.AvatarID = GameID();
    SRoomUpdate.OldRoom = m_Oldroom;
    SRoomUpdate.NewRoom = m_Room;

    if (m_SectorMgr && change)
    {
		while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
		{
			if (p)
			{
				 if (p->GameID() != GameID())
				 {
					 p->SendOpcode(ENB_OPCODE_00A0_STARBASE_ROOM_CHANGE,(unsigned char *) &SRoomUpdate, sizeof(SRoomUpdate));
					 
					 if (p->m_Room == m_Room)
					 {
						 //stimulate each avatar in the same room for this player, so we can see stationary players
						 SendStarbaseAvatarChange(p);
					 }
				 }
			}
		}
    }
}

// TODO: Broadcast the position change to all other players in the room
void Player::BroadcastPosition()
{
    Player *p = (0);
	u32 * sector_list = GetSectorPlayerList();  
	
	// This method broadcasts a player's position to all other players
    // in the same room.

    if (m_SectorMgr)
    {
		while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
		{
			if (p && p->HavePosition() &&
                p->GameID() != GameID())
            {   
                p->SendStarbaseAvatarChange(this);
            }
        }
    }
}

void Player::SendStarbaseAvatarList()
{
    Player *p = 0;
    
    if (!Active() || (!m_SectorMgr))
    {
        return;
    }
    
    u32 * sector_list = GetSectorPlayerList();
    
    StarbaseRoomChange SRoomUpdate;
    SRoomUpdate.AvatarID = GameID();
    SRoomUpdate.OldRoom = m_Oldroom;
    SRoomUpdate.NewRoom = m_Room;
    
	while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
	{        
        if (p && p->GameID() != GameID())            // Not this player
            // TODO: check action flag?
        {
            // This avatar is in the same room as this player
            // Send this player's avatar to the other player
            
            AvatarDescription avatar;
            memset(&avatar, 0, sizeof(avatar));
            m_Mutex.Lock();
            memcpy(&avatar.avatar_data, &m_Database.avatar, sizeof(avatar.avatar_data));
            avatar.AvatarID = GameID();
            avatar.unknown3 = 1.0;
            avatar.unknown4 = 1.0;
            m_Mutex.Unlock();
            
            p->SendOpcode(ENB_OPCODE_0061_AVATAR_DESCRIPTION, (unsigned char *) &avatar, sizeof(avatar));
            
            // Removes player from the room/orbs out
            p->SendOpcode(ENB_OPCODE_00A0_STARBASE_ROOM_CHANGE,(unsigned char *) &SRoomUpdate, sizeof(SRoomUpdate));

            if (m_Room != -1)
            {
                memset(&avatar, 0, sizeof(avatar));
                m_Mutex.Lock();
                memcpy(&avatar.avatar_data, &p->m_Database.avatar, sizeof(avatar.avatar_data));
                avatar.AvatarID = p->GameID();
                avatar.unknown3 = 1.0;
                avatar.unknown4 = 1.0;
                m_Mutex.Unlock();
                
                SendOpcode(ENB_OPCODE_0061_AVATAR_DESCRIPTION, (unsigned char *) &avatar, sizeof(avatar)); 
                
                SendOpcode(ENB_OPCODE_00A0_STARBASE_ROOM_CHANGE,(unsigned char *) &SRoomUpdate, sizeof(SRoomUpdate));
                
                if (p->HavePosition())
                {
                    // Send the Starbase Avatar Change packet to this client to update the other player's position
                    SendStarbaseAvatarChange(p);
                }
            }
        }
    }
}

char * Player::GetRank()
{
    static char *rank_list[] =
    {
        // TW (Enforcer)
        "Ensign","Lieutenant","Lt.Commander","Commander","Captain","Commodore","Admiral",
        // TT (Tradesman)
        "Prentice","Journeyman","Agent","Emptor","Guildsman","Magnate","Merchant Prince",
        // TE (Scout)
        "Cadet","Private","Corporal","Sergeant","Master Sergeant","Sergeant Major","Warrant Officer",
        // JW (Defender)
        "J'nai","D'nai","U'nai","Lai'shao","Dai'shao","Ten'shao","Ken'shao",
        // JT (Seeker)
        "Nan'Jeu","Zi'Jeu","Bo'Jeu","Hou'Jeu","Gong'Jeu","Wang'Jeu","Huangdi'Jeu",
        // JE (Explorer)
        "Aspirant","Initiate","Novice","Disciple","Adept","Master","Grandmaster",
        // PW (Warrior)
        "Legionaire","Centurion","Lancearate","Praefect","Legate","Consul","Proconsul",
        // PT (Privateer)
        "Quaestor","Aedile","Tribune","Praetor","Procurator","Triumvir","Imperator",
        // PE (Sentinel)
        "Inceptor","Librorum","Savant","Pedagogue","Doctrinaire","Magister","Magister Magna"
    };

    int rank_index = PlayerIndex()->RPGInfo.GetHullUpgradeLevel();

	return (rank_list[ClassIndex() * 7 + rank_index]);
}

void Player::Remove()
{
    Player * p;
    RangeListVec::iterator itrRList;
    PlayerList::iterator itrPList;

    u32 * sector_list = GetSectorPlayerList();

    if (m_PlayerIndex.GetSectorNum() == 0)
    {
        return;
    }
  
    if (m_PlayerIndex.GetSectorNum() <= 9999) //in space
    {
		p = (0);
		//now remove this player from all players who can see this player
		while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))
		{
            if (p->Active())
            {
                p->RemoveObject(GameID());
            }
		}

		if (sector_list)
		{
			p = (0);
			//now remove this player from all rangelists in this sector
			while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
			{
				if (p && p->Active())
				{
					RemovePlayerFromRangeList(p);
				}
			}
		}
	}
    else
    {
        m_Oldroom = m_Room;
        m_Room = -1;
        SendStarbaseAvatarList();
    }

    //now remove the player from the sector list
    m_SectorMgr->RemovePlayerFromSectorList(this);

	SaveHullLevelChange(ShipIndex()->GetMaxHullPoints());
	SaveEnergyLevels();
}

void Player::MarkForRemoval()
{
    m_Remove = true;
    if (m_SectorMgr)
    {
        m_SectorMgr->RemovePlayerFromSectorList(this);
    }
}

void Player::SendLoginShipData()
{
    short hull = (short) m_Database.ship_info.hull;
    SendCreate(GameID(), 1.0, hull, 1);
    
    SendSubparts(this);
    SendShipColorization(this, 8);    // send the ship color scheme
    
    SendOpcode(ENB_OPCODE_0037_CLIENT_AVATAR, (unsigned char *) &m_CreateInfo.GameID, sizeof(long));
    SendOpcode(ENB_OPCODE_0047_CLIENT_SHIP, (unsigned char *) &m_CreateInfo.GameID, sizeof(long));
    
    AvatarDescription avatar;
    memset(&avatar, 0, sizeof(avatar));
    avatar.AvatarID = GameID();  
    memcpy(&avatar.avatar_data, &m_Database.avatar, sizeof(avatar.avatar_data));
    avatar.unknown3 = 1.0;
    avatar.unknown4 = 1.0;
        
    SendOpcode(ENB_OPCODE_0061_AVATAR_DESCRIPTION, (unsigned char *) &avatar, sizeof(avatar));
    
    SendDecal(GameID(), m_Database.ship_data.decal, 2);
    SendNameDecal(GameID(), m_Database.ship_data.ship_name);
    SendRelationship(GameID(), RELATIONSHIP_FRIENDLY, false);
    
    SetStartingPosition();
    SendAdvancedPositionalUpdate(GameID(), &m_Position_info);

	// Set shields to recharge
	RechargeReactor();
	RechargeShield();
}

void Player::SendShipData(Player *player)
{
    Player *player_to_send_to = (Player*)player;
    if (player_to_send_to)
    {
        short hull = (short) m_Database.ship_info.hull;
        player_to_send_to->SendCreate(GameID(), 1.0, hull, 1);
        
        SendSubparts(player_to_send_to);
        SendShipColorization(player_to_send_to, 8);    // send the ship color scheme

        if (this == player)
        {
            SendOpcode(ENB_OPCODE_0037_CLIENT_AVATAR, (unsigned char *) &m_CreateInfo.GameID, sizeof(long));
	        SendOpcode(ENB_OPCODE_0047_CLIENT_SHIP, (unsigned char *) &m_CreateInfo.GameID, sizeof(long));
        }
        
        AvatarDescription avatar;
        memset(&avatar, 0, sizeof(avatar));
        avatar.AvatarID = GameID();  
        memcpy(&avatar.avatar_data, &m_Database.avatar, sizeof(avatar.avatar_data));
        avatar.unknown3 = 1.0;
        avatar.unknown4 = 1.0;
        
        if (player_to_send_to->AdminLevel() == 90 && player_to_send_to != this) //gives devs a bit more feedback.
        {
            player_to_send_to->SendVaMessage("Sending %s [%x] to you", Name(), GameID());
        }

        player_to_send_to->SendOpcode(ENB_OPCODE_0061_AVATAR_DESCRIPTION, (unsigned char *) &avatar, sizeof(avatar));
        
        player_to_send_to->SendDecal(GameID(), m_Database.ship_data.decal, 2);
        player_to_send_to->SendNameDecal(GameID(), m_Database.ship_data.ship_name);
        player_to_send_to->SendRelationship(GameID(), RELATIONSHIP_FRIENDLY, false);

        m_ShipIndex.Buffer()->Lock();
        
        if (m_ShipIndex.BuildCreateExtendedPacket())
        {
            player_to_send_to->SendOpcode(ENB_OPCODE_001B_AUX_DATA, 
                m_ShipIndex.m_Packet, 
                (u16)m_ShipIndex.m_PacketLength);
        }
        
        m_ShipIndex.Buffer()->Unlock();
        
        player->SendAdvancedPositionalUpdate(GameID(), &m_Position_info);
    }
}

void Player::SendSubparts(Player *player_to_send_to)
{
    if (player_to_send_to)
    {
		unsigned char subparts[128];
		memset(subparts,0,128);
		int index = 0;

		AddData(subparts, ntohl(GameID()), index);
		AddData(subparts, ntohl(4), index);
		AddDataS(subparts,"~01", index);
		index++;
		AddData(subparts, ntohl(m_Database.ship_info.profession), index);

		switch (m_Database.ship_data.race)
		{
		case RACE_JENQUAI:
			AddDataS(subparts,"~01/~03_01", index);
			index++;
			AddData(subparts, ntohl(m_Database.ship_info.engine), index);
			AddDataS(subparts,"~01/~03_02", index);
			index++;
			AddData(subparts, ntohl(m_Database.ship_info.engine), index);
			AddDataS(subparts,"~02", index);
			index++;
			AddData(subparts, ntohl(m_Database.ship_info.wing), index);
			break;

		default:
			AddDataS(subparts,"~02", index);
			index++;
			AddData(subparts, ntohl(m_Database.ship_info.wing), index);
			AddDataS(subparts,"~02/~03_01", index);
			index++;
			AddData(subparts, ntohl(m_Database.ship_info.engine), index);
			AddDataS(subparts,"~02/~03_02", index);
			index++;
			AddData(subparts, ntohl(m_Database.ship_info.engine), index);
			if (Profession() == PROFESSION_TRADER && Race() == RACE_TERRAN && PlayerIndex()->RPGInfo.GetHullUpgradeLevel() >= 5)
			{
				*((long*) &subparts[4]) = ntohl(6);
				AddDataS(subparts,"~02/~03_03", index);
				index++;
				AddData(subparts, ntohl(m_Database.ship_info.engine), index);
				AddDataS(subparts,"~02/~03_04", index);
				index++;
				AddData(subparts, ntohl(m_Database.ship_info.engine), index);
			}
			break;
		}

        player_to_send_to->SendOpcode(ENB_OPCODE_00B4_SUBPARTS, subparts, index);	
    }
}

void Player::SendPlayerInfo()
{
	for(int i=0; i<96; i++) 
	{
        SendItemBase(PlayerIndex()->SecureInv.Item[i].GetItemTemplateID());
	}

	PlayerIndex()->VendorInv.Clear();
	PlayerIndex()->SetMusicID(-1);

	SendAuxPlayerExtended();

    //This actually invokes sending the AuxPlayer packet
    //Therefore do it after the initial is sent
	g_ServerMgr->m_PlayerMgr.RequestGroupAux(GroupID(), GameID());
}

void Player::SendShipInfo(long NewGameID, long SpeedFactor)
{
	int x, class_index = ClassIndex();
    //float stat_val;

   	ShipIndex()->SetGameID(NewGameID);

//	m_Stats.ResetStats();

	for(x=0;x<40;x++) 
	{
        SendItemBase(ShipIndex()->Inventory.CargoInv.Item[x].GetItemTemplateID());
	}

	for(x=0;x<20;x++) 
	{
		// Only need to do this once per login
        SendItemBase(ShipIndex()->Inventory.EquipInv.EquipItem[x].GetItemTemplateID());
		SendItemBase(ShipIndex()->Inventory.AmmoInv.Item[x].GetItemTemplateID());
	}

	ShipIndex()->SetTargetGameID(-1);
    ShipIndex()->SetPrivateWarpState(PRIVATE_WARP_AVAILABLE); 
    ShipIndex()->SetWarpTriggerTime(GetNet7TickCount());

	if (SpeedFactor == 2)
	{
		float speed = ShipIndex()->GetMaxSpeed();
		ShipIndex()->SetMaxSpeed(speed*2.0f);
		ShipIndex()->SetMinSpeed(-speed);
		ShipIndex()->SetAcceleration(ShipIndex()->GetAcceleration()*2.0f);
	}

    SendAuxShipExtended();

	m_Effects.SendEffects(this);
}


void Player::SendAuxPlayer()
{	
	if (PlayerIndex() == NULL)
	{
		LogMessage("ERROR: Player Index not found for: %s", Name());
		return;
	}

	if (PlayerIndex()->BuildPacket())
	{
		SendOpcode(ENB_OPCODE_001B_AUX_DATA, PlayerIndex()->PacketBuffer, PlayerIndex()->PacketSize);
		//DumpBufferToFile(PlayerIndex()->PacketBuffer, PlayerIndex()->PacketSize, "auxplayerdat.dat",true);
	}
}

void Player::SendAuxPlayerExtended()
{
	if (PlayerIndex() == NULL)
	{
		LogMessage("ERROR: Player Index not found for: %s", Name());
		return;
	}

	if (PlayerIndex()->BuildExtendedPacket())
	{
        //assumes login only
		SendOpcode(ENB_OPCODE_001B_AUX_DATA, PlayerIndex()->PacketBuffer, PlayerIndex()->PacketSize); 
		//DumpBufferToFile(PlayerIndex()->PacketBuffer, PlayerIndex()->PacketSize, "auxplayerdat.dat",true);
        PlayerIndex()->ClearFlags();
	}

}

void Player::SendAuxShip(Player * other)
{
    /* If no Diff exists, exit */
    if (!m_ShipIndex.HasDiff())
    {
        return;
    }
    
    /* Block anyone else from using the buffer */
    m_ShipIndex.Buffer()->Lock();
    
    m_ShipIndex.BuildDiff();

	if ( m_ShipIndex.m_DiffLength == 0)
	{
		/* Unblock buffer */
		m_ShipIndex.Buffer()->Unlock();
		return;
	}
    
    SendOpcode(ENB_OPCODE_001B_AUX_DATA, m_ShipIndex.m_Diff, m_ShipIndex.m_DiffLength);
    
    /* If we need to copy this entire packet to someone else, do it now */
    if (other)
    {
        other->SendOpcode(ENB_OPCODE_001B_AUX_DATA, m_ShipIndex.m_Diff, m_ShipIndex.m_DiffLength);
    }
    
    /* It is possible that no changes exist for the create and click packet
    Its pointless to iterate though the range list if neither packet is valid */
    
    if (m_ShipIndex.m_CreateDiffLength || m_ShipIndex.m_ClickDiffLength)
    {
        Player *p;
        
        m_Mutex.Lock();

		p = (0);
		while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))
		{         
            /* Dont repeat packets to the same player */
            if (p == this || p == other)
            {
                continue;
            }
            
            /* This player is in the range list, he recieves the create diff always */
            if (m_ShipIndex.m_CreateDiffLength)
            {
                p->SendOpcode(ENB_OPCODE_001B_AUX_DATA, 
                    m_ShipIndex.m_CreateDiff, m_ShipIndex.m_CreateDiffLength);
            }
            
            /* If they have clicked and a click diff packet exists */
            if (IsClickedBy(p) && m_ShipIndex.m_ClickDiffLength)
            {
                p->SendOpcode(ENB_OPCODE_001B_AUX_DATA, 
                    m_ShipIndex.m_ClickDiff, m_ShipIndex.m_ClickDiffLength);
            }
        }
        
        m_Mutex.Unlock();
    }
    
    /* Unblock buffer */
    m_ShipIndex.Buffer()->Unlock();
    
}

void Player::SendAuxShipExtended()
{
	if (m_ShipIndex.BuildExtendedPacket())
	{
        //assume this is only sent at login
		SendOpcode(ENB_OPCODE_001B_AUX_DATA, m_ShipIndex.m_Packet, m_ShipIndex.m_PacketLength);
        m_ShipIndex.ClearFlags();
	}
}

void Player::SendAuxManu(bool TwoBitFlags)
{
	if (ManuIndex()->BuildPacket(TwoBitFlags))
	{
        SendOpcode(ENB_OPCODE_001B_AUX_DATA, ManuIndex()->PacketBuffer, ManuIndex()->PacketSize);
		//DumpBufferToFile(ManuIndex()->PacketBuffer, ManuIndex()->PacketSize, "auxmanu.dat",true);
	}
}

void Player::SendShipColorization(Player * player_to_send_to, int count)
{
    if (player_to_send_to)
    {
        m_Mutex.Lock();

        if (count > MAX_COLORIZATION_ITEMS)
        {
            count = MAX_COLORIZATION_ITEMS;
        }

        Colorization colorization;
        colorization.GameID = GameID();
        colorization.ItemCount = count;

        colorization.item[0].metal = m_Database.ship_data.HullPrimaryColor.metal;
        colorization.item[0].HSV[0] = m_Database.ship_data.HullPrimaryColor.HSV[0];
        colorization.item[0].HSV[1] = m_Database.ship_data.HullPrimaryColor.HSV[1];
        colorization.item[0].HSV[2] = m_Database.ship_data.HullPrimaryColor.HSV[2];
        colorization.item[1].metal = m_Database.ship_data.HullSecondaryColor.metal;
        colorization.item[1].HSV[0] = m_Database.ship_data.HullSecondaryColor.HSV[0];
        colorization.item[1].HSV[1] = m_Database.ship_data.HullSecondaryColor.HSV[1];
        colorization.item[1].HSV[2] = m_Database.ship_data.HullSecondaryColor.HSV[2];

        colorization.item[2].metal = m_Database.ship_data.ProfessionPrimaryColor.metal;
        colorization.item[2].HSV[0] = m_Database.ship_data.ProfessionPrimaryColor.HSV[0];
        colorization.item[2].HSV[1] = m_Database.ship_data.ProfessionPrimaryColor.HSV[1];
        colorization.item[2].HSV[2] = m_Database.ship_data.ProfessionPrimaryColor.HSV[2];
        colorization.item[3].metal = m_Database.ship_data.ProfessionSecondaryColor.metal;
        colorization.item[3].HSV[0] = m_Database.ship_data.ProfessionSecondaryColor.HSV[0];
        colorization.item[3].HSV[1] = m_Database.ship_data.ProfessionSecondaryColor.HSV[1];
        colorization.item[3].HSV[2] = m_Database.ship_data.ProfessionSecondaryColor.HSV[2];

        colorization.item[4].metal = m_Database.ship_data.WingPrimaryColor.metal;
        colorization.item[4].HSV[0] = m_Database.ship_data.WingPrimaryColor.HSV[0];
        colorization.item[4].HSV[1] = m_Database.ship_data.WingPrimaryColor.HSV[1];
        colorization.item[4].HSV[2] = m_Database.ship_data.WingPrimaryColor.HSV[2];
        colorization.item[5].metal = m_Database.ship_data.WingSecondaryColor.metal;
        colorization.item[5].HSV[0] = m_Database.ship_data.WingSecondaryColor.HSV[0];
        colorization.item[5].HSV[1] = m_Database.ship_data.WingSecondaryColor.HSV[1];
        colorization.item[5].HSV[2] = m_Database.ship_data.WingSecondaryColor.HSV[2];

        colorization.item[6].metal = m_Database.ship_data.EnginePrimaryColor.metal;
        colorization.item[6].HSV[0] = m_Database.ship_data.EnginePrimaryColor.HSV[0];
        colorization.item[6].HSV[1] = m_Database.ship_data.EnginePrimaryColor.HSV[1];
        colorization.item[6].HSV[2] = m_Database.ship_data.EnginePrimaryColor.HSV[2];
        colorization.item[7].metal = m_Database.ship_data.EngineSecondaryColor.metal;
        colorization.item[7].HSV[0] = m_Database.ship_data.EngineSecondaryColor.HSV[0];
        colorization.item[7].HSV[1] = m_Database.ship_data.EngineSecondaryColor.HSV[1];
        colorization.item[7].HSV[2] = m_Database.ship_data.EngineSecondaryColor.HSV[2];

        size_t size = ((char *) &colorization.item[count]) - ((char *) &colorization);

        player_to_send_to->SendOpcode(ENB_OPCODE_0011_COLORIZATION, (unsigned char *) &colorization, size);
        m_Mutex.Unlock();
    }
}

void Player::StoreDockingCoords(float *position, float *heading)
{
    m_DockHeading[0] = heading[0];
    m_DockHeading[1] = heading[1];
    m_DockHeading[2] = heading[2];
    m_DockCoords[0] = position[0];
    m_DockCoords[1] = position[1];
    m_DockCoords[2] = position[2];
}

void Player::LogDockCoords()
{ 
    m_LogDockCoords = true; 
    StoreDockingCoords(Position(), Heading());
}

bool Player::RestoreDockingCoords()
{
    if (m_DockCoords[0] == 0 && m_DockCoords[1] == 0 && m_DockCoords[2] == 0)
    {
        return false;
    }
    else
    {
        m_Position_info.Velocity[0] =- m_DockHeading[0];
        m_Position_info.Velocity[1] =- m_DockHeading[1];
        m_Position_info.Velocity[2] =- m_DockHeading[2];
        SetPosition(m_DockCoords);
        LevelOrientation();
        m_DockCoords[0] = m_DockCoords[1] = m_DockCoords[2] = 0.0f;
        m_DockHeading[0] = m_DockHeading[1] = m_DockHeading[2] = 0.0f;
        return true;
    }
}

u16 Player::UpdatePositionFromMVAS(float *position, float *heading, bool heading_sent)
{
	unsigned long current_tick = GetNet7TickCount();
	bool tick_valid = true;
	float Range = 0.0f;
	u16 frequency = 2;

    if (m_LogDockCoords && heading_sent)
    {
		m_SendDockPos = true;
		m_SentDockPos = false;
        StoreDockingCoords(position, heading);
        m_LogDockCoords = false; //first received impulse since docking signal sent.
    }

	if (WarpDrive() || m_FollowObject || (!ReceivedMVAS() && PosX() == 0.0f && PosY() == 0.0f && PosZ() == 0.0f))
	{
		return (frequency);
	}

    m_LastAccessTime = current_tick;
    m_ReceivedMovement = true;

	if (Active() && !WarpDrive())
	{
		frequency = 1;
		
		/*if (m_LastRead != 0) 
		{
            Range = RangeFrom(position);

			if (Range > ((float)(current_tick - m_LastRead) * ShipIndex()->BaseStats.GetSpeed() * 4))
			{
				LogMessage("Poll position update not in range of ship. Would Reject\n");
				tick_valid = false;
			}
		}*/

#ifdef MOVEMENT_DEBUG
        LogDebug("Received MVAS from %s[0x%x]: %.2f %.2f %.2f\n", Name(), GameID(), PosX(), PosY(), PosZ());
#endif
		
		if (tick_valid)
		{
            m_Mutex.Lock();
			if (m_ReceivedMVAS == false)
			{
				LogMessage("MVAS synched and locked in for %s[0x%x]: x %.2f y %.2f z %.2f\n", Name(), GameID(), PosX(), PosY(), PosZ());
				m_ReceivedMVAS = true;

				if (g_Debug && AdminLevel() == 90)
				{
					SendVaMessage("MVAS: MVAS synched and locked.");
				}
			}

            SetPosition(position); //take player position from MVAS feed
            m_Mutex.Unlock();

            if (heading_sent) //take new heading from MVAS feed
            {
                SetVelocityVector(heading); //take heading from MVAS feed
                LevelOrientation(); //postfix our orientation from the heading data
            }

            UpdateLastPosition(current_tick);
            
            /*if (m_MyDebugPlayer)
            {
                float *ori = Orientation();
                m_MyDebugPlayer->SetPosition(position);
                m_MyDebugPlayer->SetOrientation(ori[0], ori[1], ori[2], ori[3]);
                m_MyDebugPlayer->SetVelocity(m_Velocity);
                m_MyDebugPlayer->m_Position_info.SetSpeed = m_Velocity*0.001f;
                m_MyDebugPlayer->SendLocationAndSpeed(false);
            }*/
		}
        else //this update wasn't valid ... hacking most likely - use normal position calculation
        {
            SendLocationAndSpeed(true);
        }
	}

	return frequency;
}

float Player::FollowTarget()
{
    //get target
    Object *obj = GetObjectManager()->GetObjectFromID(ShipIndex()->GetTargetGameID());

    if (obj && ((obj->ObjectType() != OT_MOB && obj->ObjectType() != OT_PLAYER)) )
    {
        m_FollowObject = false;
    }
    else if (obj && m_FollowObject)
    {
        //update orientation to face target
        Face(obj);

        float target_velocity = (obj->Velocity() > ShipIndex()->GetMaxSpeed()) ? 
                ShipIndex()->GetMaxSpeed() : obj->Velocity(); //make sure we can't exceed top speed.

        float range = obj->RangeFrom(Position());

        if (range < 10.0f)
        {
            m_Velocity = 0.0f;
        }
        else if (range < 200.0f)
        {
            m_Velocity = (target_velocity * 0.25f);
        }
        else if (range > 1500.0f)
        {
            m_Velocity = ShipIndex()->GetMaxSpeed();
        }
        else if (range > 3000.0f)
        {
            StopFollowing();
        }
        else
        {
            m_Mutex.Lock();
            //update velocity to match target
            m_Velocity = target_velocity;
            m_Mutex.Unlock();
        }
    }
    else
    {
        StopFollowing();
    }

	//interrupt any skill that breaks on motion
	bool OnMotion = false, Ignored = false;
	if(m_CurrentSkill && m_CurrentSkill->SkillInterruptable(&OnMotion, &Ignored, &Ignored))
	{
		if(OnMotion)
		{
			m_CurrentSkill->InterruptSkillOnMotion(m_Velocity);
		}
	}

    return (m_Velocity);
}

void Player::StopFollowing()
{
    if (m_FollowObject && m_Velocity != 0.0f) SendContrailsRL(false);
    m_Mutex.Lock();
    m_FollowObject = false;
    m_Velocity = 0.0f;
    m_SetUpdate = 1;
    m_Mutex.Unlock();
}

void Player::UpdateLastPosition(unsigned long current_tick)
{
    m_Mutex.Lock();
    m_LastRead = current_tick;
    m_LastPos[0] = PosX();
    m_LastPos[1] = PosY();
    m_LastPos[2] = PosZ();
    m_LastVelocity = m_Velocity;
    m_Mutex.Unlock();
}

bool Player::CheckUpdateConditions(unsigned long current_tick)
{
	if (!Active() || current_tick == m_LastUpdate)
	{
		return false;
	}

    if (m_DebugPlayer)
    {
        return false;
    }
	
	if (m_LastUpdate == 0)
	{
		m_LastUpdate = current_tick;
		return false;
	}

    return true;
}

void Player::CalcNewHeading(float tdiff, bool turn)
{
    /*if (m_YInput != 0.0f)
    {
        m_YHeading += m_YInput*tdiff*ShipIndex()->GetMaxTiltRate(); 
        if (m_YHeading > 1.221731f) m_YHeading =   1.221731f; // ShipIndex()->GetMaxTiltAngle()) m_YHeading =  ShipIndex()->GetMaxTiltAngle(); //TODO: set this value in Aux, currently zero
        if (m_YHeading < -1.221731f) m_YHeading = -1.221731f; //-ShipIndex()->GetMaxTiltAngle()) m_YHeading = -ShipIndex()->GetMaxTiltAngle();
    }

    if (m_ZInput != 0.0f)
    {
        m_ZHeading += m_ZInput*tdiff*ShipIndex()->GetMaxTurnRate(); 
        if (m_ZHeading > 2.0f*3.14159f) m_ZHeading -= 2.0f*3.14159f;
        if (m_ZHeading < 0.0f)          m_ZHeading += 2.0f*3.14159f;
    }*/

    float rot_Z[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float rot_Y[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float result[4];
    
    if (m_ZInput != 0.0f)
    {
        rot_Z[2] = -m_ZInput*tdiff*0.42f/883.0f*1000.0f;
        m_ReceivedMovement = true;
    }
    
    if (m_YInput != 0.0f)
    {
        rot_Y[1] = -m_YInput*tdiff*0.42f/883.0f*1000.0f;
        m_ReceivedMovement = true;
    }
    
    if (m_ReceivedMovement)
    {
        Quat4fMul(rot_Z, rot_Y, result);
        Quat4fMul(Orientation(), result, Orientation());
        Quat4fNormalize(Orientation());
        SetHeading();
    }
}

//This updates the instantaneous velocity for the packets (m_Velocity)
//and also calculates the average velocity over the time period for position calculation
float Player::CalcVelocity(float tdiff)
{
    float av_speed;
   
    if (m_Accelerating)
    {
        float endspeed_time = 0.0f;
        float final_velocity = 0.0f;
        float acc = 1.0f;

        //now calculate time to end speed
        switch (m_Thrust)
        {
        case 1: //thrust forward
            endspeed_time = (ShipIndex()->GetMaxSpeed() - m_Velocity) / ShipIndex()->GetAcceleration();
            final_velocity = ShipIndex()->GetMaxSpeed();
            break;
        case 0:
            if (m_Velocity > 0) acc = -1.0f;
            endspeed_time = -acc * m_Velocity / ShipIndex()->GetAcceleration();
            final_velocity = 0.0f;
            break;
        case -1:
            acc = -1.0f;
            endspeed_time = (ShipIndex()->GetMinSpeed() - m_Velocity) / -ShipIndex()->GetAcceleration();
            final_velocity = ShipIndex()->GetMinSpeed();
            break;
        }

        av_speed = m_Velocity + 0.5f*acc*tdiff*ShipIndex()->GetAcceleration(); //starting velocity + 1/2 of velocity increase

        //now calculate average speed over time period
        if (endspeed_time < tdiff)
        {
            //we finished accelerating in this time period
            av_speed -= 0.5f*acc*ShipIndex()->GetAcceleration()*(tdiff - endspeed_time); //subtract the velocity contribution from any acceleration after full speed reached
            m_Velocity = final_velocity;
            m_Accelerating = false;
        }
        else
        {
            m_Velocity = m_Velocity + acc*tdiff*ShipIndex()->GetAcceleration();
        }
    }
    else
    {
        av_speed = m_Velocity;
    }

	//interrupt any skill that breaks on motion
	bool OnMotion = false, Ignored = false;
	if(m_CurrentSkill && m_CurrentSkill->SkillInterruptable(&OnMotion, &Ignored, &Ignored))
	{
		if(OnMotion && ObjectIsMoving())
		{
			m_CurrentSkill->InterruptSkillOnMotion(m_Velocity);
		}
	}

    return (av_speed);
}

void Player::CheckNavs()
{
	SectorManager *sm = GetSectorManager();
	ObjectManager *om = GetObjectManager();
	if (sm && om)
	{
		if (m_FoundAllSectorNavs[sm->GetSectorNumber()] || m_Hijackee != 0) //no need to check if we've explored everything, or we've hijacked something
		{
			return;
		}

		//begin sweep of navs in sector to see if we're in expose or explore range of any navs
		m_FoundAllSectorNavs[sm->GetSectorNumber()] = !om->CheckNavRanges(this);
	}
}

long *Player::ExploredNavList()
{
    return (m_NavsExplored);
}

long *Player::ExposedNavList()
{
    return (m_NavsExposed);
}

void Player::CalcNewPosition(unsigned long current_tick, bool turn)
{
	float tdiff;
    float av_speed;

    if (turn && m_FollowObject)
    {
        m_FollowObject = false;
    }

    if (!CheckUpdateConditions(current_tick))
    {
        return;
    }
    
    tdiff = (float)(current_tick - m_LastUpdate)/1000.0f;

    if (m_FollowObject)
    {
        av_speed = FollowTarget();
    }
    else
    {
        if (!IS_PLAYER(m_MVAS_index))
        {
            CalcNewHeading(tdiff, turn);
        }
        //Now perform the acceleration calculations to update player velocity and get average speed of tdiff period
        av_speed = CalcVelocity(tdiff);
    }
       
    if (av_speed != 0.0f)
    {
        ExtrapolatePosition(av_speed, tdiff); 
        CheckNavs(); //this checks to see if we're in range of any unexposed or unexplored navs
        if (CheckBoundaries())
        {
            SendVaMessage("Reached boundary.");
            TerminateWarpGroup();
        }
        if (m_WarpDrive)
        {
            m_ReceivedMovement = true;
            CheckWarpNavigation();
        }
    }

    m_LastUpdate = current_tick;
}

void Player::SetVelocity(float velocity)
{
	m_Velocity = velocity;
	
	//interrupt any skill that breaks on motion
	bool OnMotion = false, Ignored = false;
	if(m_CurrentSkill && m_CurrentSkill->SkillInterruptable(&OnMotion, &Ignored, &Ignored))
	{
		if(OnMotion)
		{
			m_CurrentSkill->InterruptSkillOnMotion(m_Velocity);
		}
	}
}

void Player::Move(int type)
{
    if (m_FollowObject)
    {
        m_FollowObject = false;
        //bring ship to halt.
        m_Velocity = 0.0f;
        m_SetUpdate = 1;
    }

    m_ReceivedMovement = true;
	m_Arrival_Flag = false;

    unsigned long current_tick = GetNet7TickCount();
        
    switch(type)
    {
    case 0:             //accelerate via mouse
    case 2:				//forward thrust button
        CalcNewPosition(current_tick);
        m_Thrust = 1;
        m_Accelerating = true;
        m_Position_info.SetSpeed = ShipIndex()->GetMaxSpeed()*0.001f;
        break;
    case 1:             //Reverse thrust on 'z' keypress
    case 3:				//Reverse thrust
        CalcNewPosition(current_tick);
        m_Thrust = -1;
        m_Accelerating = true;
        m_Position_info.SetSpeed = ShipIndex()->GetMinSpeed()*0.001f;
        break;
    case 4:             //kill thrust, start decelerating
        CalcNewPosition(current_tick);
        m_Thrust = 0;
        m_Accelerating = true;
        m_Position_info.SetSpeed = 0.0f;
        break;
    case 5:
        //do nothing
        break;

    case 12:
        //follow something
        if (!m_WarpDrive && !m_Prospecting && !ShipIndex()->GetIsIncapacitated())
        {
            m_FollowObject = true;
        }
        break;
        
    default:
        LogMessage("** Received strange move impulse from %s = %d\n", Name(), type);
        break;
    }

    SetLastAccessTime(current_tick);
}

void Player::MoveToward(Object *obj, float speed)
{
    Face(obj);
    m_Velocity = speed;
    SendLocationAndSpeed(true);

	//interrupt any skill that breaks on motion
	bool OnMotion = false, Ignored = false;
	if(m_CurrentSkill && m_CurrentSkill->SkillInterruptable(&OnMotion, &Ignored, &Ignored))
	{
		if(OnMotion)
		{
			m_CurrentSkill->InterruptSkillOnMotion(m_Velocity);
		}
	}
}

void Player::UpdatePositionInfo()
{
    if (m_WarpDrive)
    {
        m_Position_info.Bitmask = 0x01;  
    }
    else if (m_Velocity == 0.0f)
    {
        m_Position_info.Bitmask = 0x00;
    }
    else
    {
        m_Position_info.Bitmask = 0x2F;
    }
    
    m_Position_info.CurrentSpeed = m_Velocity*0.001f;
    m_Position_info.MovementID = m_MovementID;
    m_Position_info.DesiredY = 0;
    m_Position_info.DesiredZ = 0;
    m_Position_info.Acceleration = ShipIndex()->GetAcceleration()*0.001f;
}

void Player::SendStartMovementRefresh()
{
	m_MovementID = 5;
	UpdatePositionInfo();
	SendAdvancedPositionalUpdate(GameID(), &m_Position_info);
}

void Player::SendLocationAndSpeed(bool include_player)
{
    if (Active())
    {
        UpdatePositionInfo();

        //use setupdate to force local client player update.
        if (m_SetUpdate > 0)
        {
            m_SetUpdate--;
            include_player = true;
        }

		if (m_Hijackee != 0)
		{
			Object *obj = GetObjectManager()->GetObjectFromID(Hijackee());
			if (obj)
			{
				obj->SetPosition(Position());
				obj->SetOrientation(Orientation());
				obj->SetVelocity(m_Velocity);
				obj->SetMovementID(m_MovementID);
				obj->SendLocationAndSpeed(true);
				if (include_player || m_WarpDrive)
				{
					SendAdvancedPositionalUpdate(GameID(), &m_Position_info);
				}
				return;
			}
		}

        SendToVisibilityList(include_player);

#ifdef MOVEMENT_DEBUG
        // Too many printouts to use without a #define
        LogDebug("Pos Update [%c][%d] sent for %s [%x]: %.2f %.2f %.2f [vel %.2f] [mask %x]\n", include_player ? '*' : ' ',
            m_Position_info.MovementID,
            Name(), GameID(), 
			PosX(), PosY(), PosZ(),
            m_Position_info.CurrentSpeed, m_Position_info.Bitmask);
#endif
	}
}

bool Player::HijackObject(Object *obj)
{
	if (!obj || obj->ObjectType() == OT_PLAYER)
    {
        return false;
    }

    //first remove us from everyone's range lists (not ours)
	BlankRangeList();

    //now set Hijackee
	m_Hijackee = obj->GameID();
    obj->SetPlayerHijack(this);

    //now set position to Hijackee
    SetPosition(obj->Position());
    //ok, ready to carry on as usual now!

	return true;
}

char *Player::GetHijackeeName()
{
	char *retval = Name();
    if (GetObjectManager() && Hijackee() > 0)
    {
		Object *obj = GetObjectManager()->GetObjectFromID(Hijackee());
		if (obj)
		{
			retval = obj->Name();
		}
	}
	
	return retval;
}

void Player::SetActive(bool flag)
{
    m_Mutex.Lock();
    if (Active() == false && flag == true)
    {
        SendLocationAndSpeed(true);
        SetLastAccessTime(GetNet7TickCount());
    }
    m_Active = flag;
    m_Mutex.Unlock();
}

//warp methods
void Player::BlankWarpNavs()
{
    memset(m_WarpNavs, 0, sizeof(m_WarpNavs));
    m_WarpNavIndex = 0;
    m_WarpNavCount = 0;
}

void Player::SetupWarpNavs(short navs, long *target_id)
{
    BlankWarpNavs();

    if (navs > 0)
	{
		for(int i = 0; i < navs; i++)
		{
			m_WarpNavs[i] = target_id[i];
		}
	}

    m_WarpNavCount = navs;
}

void Player::InitiateWarpBroadcast()
{
    ImmobilisePlayer();

	ShipIndex()->SetWarpTriggerTime(GetNet7TickCount());
    ShipIndex()->SetPrivateWarpState(PRIVATE_WARP_INITIATE_WARP);
    ShipIndex()->SetGlobalWarpState(GLOBAL_WARP_INITIATE_WARP);

    //This now sends to the range list as needed
    SendAuxShip();

	//TODO: investigate how to get the effects properly - we shouldn't need to manually drive them
	//      there is some problem with how the AuxShip data is sent (I think).
	ObjectEffect WarpEffect;
	WarpEffect.Bitmask = 3;
	WarpEffect.GameID = GameID();
	WarpEffect.EffectDescID = 443; // Warp effect
	m_WarpEffect = GetSectorNextObjID();
	WarpEffect.EffectID = m_WarpEffect;
	WarpEffect.TimeStamp = GetNet7TickCount();

	SendObjectEffectRL(&WarpEffect);
}

void Player::StartWarpBroadcast()
{
    //Dima: Changed to first send the warp effect, and THEN send the aux
    //      There may be a small delay before the aux packet is sent

	ObjectEffect WarpEffect;
	WarpEffect.Bitmask = 3;
	WarpEffect.GameID = GameID();
	WarpEffect.EffectDescID = 23; // Warp effect
	WarpEffect.EffectID = GetSectorNextObjID();
	WarpEffect.TimeStamp = GetNet7TickCount();

	SendObjectEffectRL(&WarpEffect);
	
    ShipIndex()->SetPrivateWarpState(PRIVATE_WARP_SHAKE);
    ShipIndex()->SetGlobalWarpState(GLOBAL_WARP_ENGINE_TRAIL);
    ShipIndex()->SetEngineThrustState(1);

    //This now sends to the range list as needed
    SendAuxShip();
}

void Player::SendEndWarpBroadcast()
{
    ShipIndex()->SetPrivateWarpState(PRIVATE_WARP_END_NORMALLY);
    ShipIndex()->SetGlobalWarpState(GLOBAL_WARP_END_WARP);
	ShipIndex()->SetWarpTriggerTime(GetNet7TickCount() + (u32)m_Stats.GetStat(STAT_WARP_RECOVERY));
    ShipIndex()->SetEngineThrustState(0);

	//now cancel the warp effect:
	RemoveEffectRL(m_WarpEffect);

    //This now sends to the range list as needed
    SendAuxShip();
}

void Player::PrepareForWarp()
{
	bool Ignored = false, OnAction = false;
    m_Mutex.Lock();
    m_Velocity = 0;
    m_Position_info.SetSpeed = 0.0f;
    m_Position_info.RotZ = 0.0f;
    m_Position_info.RotY = 0.0f;
    m_Thrust = 0;
    m_Accelerating = false;
    m_FollowObject = false;

	//if cloaked remove cloak (ranks < 6 only)
	if(m_CurrentSkill && m_CurrentSkill->SkillInterruptable(&Ignored, &Ignored, &OnAction))
	{
		if(OnAction)
		{
			m_CurrentSkill->InterruptSkillOnAction(WARPING);
		}
	}

	m_Mutex.Unlock();
    
    if (!m_WarpDrive && !ShipIndex()->GetIsIncapacitated())
    {
		if (m_WarpNavCount > 0)
		{
			//find last object on nav list
			ObjectManager *om = GetObjectManager();
			if (om)
			{
				Object *obj = om->GetObjectFromID(m_WarpNavs[m_WarpNavCount - 1]);
				if (obj)
				{
					m_DistToNav = obj->RangeFrom(Position());
					if (m_DistToNav < 4000.0f)
					{
						SendVaMessageC(17,"Target too close for warp drive.");
						return;
					}
					else
					{
						Face(obj);
					}
				}
			}
		}

        //tell movement code we have stopped
        CalcNewPosition(GetNet7TickCount());

        LevelOrientation();

        m_Mutex.Lock();
        m_WarpDrive = true;
        m_Mutex.Unlock();
        
        SendLocationAndSpeed(true);
        
        InitiateWarpBroadcast();
        AbortProspecting(true,true);
        m_WarpBroadcastTime = (u32)m_Stats.GetStat(STAT_WARP_RECOVERY) + GetNet7TickCount(); //TODO: Get warp charge time from database         
		m_Arrival_Flag = false;
    }
}

void Player::FreeWarpDrain()
{
	//work out how long we have to warp based on current reactor level
	float energy = GetEnergyValue();

	if (m_Equip[2].GetItemInstance()->EngineFreeWarpDrain <= 0)
	{
		LogMessage("Setting Engine Freewarp drain to 20.0f\n");
		m_Equip[2].GetItemInstance()->EngineFreeWarpDrain = 20.0f;
	}

	long time_to_drain = (long)((energy/m_Equip[2].GetItemInstance()->EngineFreeWarpDrain)*1000.0f);

	DrainReactor(time_to_drain, energy);
	//at this rate we'll have to terminate warp at time_to_drain secs.
    m_WarpTime = time_to_drain + GetNet7TickCount();
}

void Player::StartWarp()
{
    if (m_WarpDrive)
    {
        StartWarpBroadcast();
		// Get the current speed as a group wide
        m_Velocity = g_ServerMgr->m_PlayerMgr.GetGroupWarpSpeed(this) + 0.5f;
		if (m_Velocity < 0.0f || m_Velocity > 10000.0f) m_Velocity = (float)ShipIndex()->CurrentStats.GetWarpSpeed();
		if (m_Velocity < 0.0f || m_Velocity > 10000.0f) m_Velocity = 3500.0f; //emergency! Just use a value of 3500 if everything's gone wrong.

        SendLocationAndSpeed(true);
        m_WarpBroadcastTime = 0;

		ObjectManager *om = GetObjectManager();
        
        if (m_WarpNavs[0] != 0 && om)
        {
            Object *obj = om->GetObjectFromID(m_WarpNavs[0]);
            if (obj)
            {
                Face(obj);
                ShipIndex()->SetTargetGameID(obj->GameID());
                SendAuxShip();
                obj->OnTargeted(this);
            }
            else
            {
                TerminateWarpGroup();
            }
        }
        else
        {
            FreeWarpDrain();
        }
    }
}

void Player::UpdateWarpNavigation()
{
    if (m_WarpDrive)
    {
        m_WarpNavIndex++;
		ObjectManager *om = GetObjectManager();
        if (m_WarpNavs[m_WarpNavIndex] != 0 && om)
        { 
            SendWarpIndex(m_WarpNavIndex);
            Object *obj = om->GetObjectFromID(m_WarpNavs[m_WarpNavIndex]);
			if (obj)
			{
				Face(obj);
				ShipIndex()->SetTargetGameID(obj->GameID());
				SendAuxShip();
				obj->OnTargeted(this);
				m_Velocity = g_ServerMgr->m_PlayerMgr.GetGroupWarpSpeed(this) + 0.5f; //restore speed if required
			}
        }
        else
        {
            TerminateWarpGroup();
        }
    }
}

void Player::CheckWarpNavigation()
{
    if (m_WarpDrive && m_WarpNavs[m_WarpNavIndex] != 0)
    {
		ObjectManager *om = GetObjectManager();
		if (om)
		{
			Object *obj = om->GetObjectFromID(m_WarpNavs[m_WarpNavIndex]);
			if (obj)
			{
				float multiplier = 0.98f;
				if (m_WarpNavs[m_WarpNavIndex+1] == 0) multiplier = 0.8f;
				float warp_nav_distance = obj->RangeFrom(Position());

				if (warp_nav_distance > m_DistToNav) m_OverrunCount++;

				if (m_OverrunCount < 3 && m_WarpNavs[m_WarpNavIndex+1] == 0 && (warp_nav_distance < m_Velocity)) //ensure we reach the target the tick after next
				{
					//we want to appear about 100.0f away from the target
					//warp_nav_distance - 100 = dist to target dest.
					m_Velocity = (warp_nav_distance - 100.0f);
					m_DistToNav = warp_nav_distance;
					m_OverrunCount = 3;
					return;
				}

				//if we're faster than about 3500, then we want to reduce speed as we approach the nav.
				if (m_Velocity > 3500.0f && warp_nav_distance < m_Velocity * 3.5f)
				{
					m_Velocity = m_Velocity * multiplier; //gradually slow down for navs
				}

				if (m_OverrunCount >= 3 || ( warp_nav_distance < 5000.0f && (warp_nav_distance > m_DistToNav || warp_nav_distance < 500.0f )) )
				{
					om->Explored(this, obj); //force exploration of nav
					m_OverrunCount = 0;
					UpdateWarpNavigation();
				}
				else
				{
					m_DistToNav = warp_nav_distance;
				}
			}
		}
		else
		{

			TerminateWarpGroup();
		}
	}
    else
    {
        if (GetNet7TickCount() > m_WarpTime)
        {
            TerminateWarpGroup();
        }
    }
}

void Player::TerminateWarpGroup(bool player_forced)
{
	if (g_ServerMgr->m_PlayerMgr.CheckGroupFormation(this))
	{
		// Termate all warps
		for(int x=0;x<6;x++)
		{
			int PlayerID = g_ServerMgr->m_PlayerMgr.GetMemberID(this->GroupID(), x);
			if (PlayerID > 0)
			{
				Player* pid = g_ServerMgr->m_PlayerMgr.GetPlayer(PlayerID);

				if (g_ServerMgr->m_PlayerMgr.CheckGroupFormation(pid))
				{
					pid->TerminateWarp(player_forced);
				}
			}
		}
	}
	else
	{
		TerminateWarp(player_forced);
	}
}

void Player::TerminateWarp(bool player_forced)
{
    if (m_WarpDrive)
    {
        if (player_forced)
        {
            CalcNewPosition(GetNet7TickCount());
        }
        SendEndWarpBroadcast();
        m_Velocity = 0;
        SendLocationAndSpeed(true);
        SendWarpIndex(-1);
        RemobilisePlayer();
	    ShipIndex()->SetPrivateWarpState(PRIVATE_WARP_TARGET_REACHED);
	    ShipIndex()->SetGlobalWarpState(GLOBAL_WARP_END_WARP);
		ShipIndex()->SetWarpTriggerTime(GetNet7TickCount() + (u32)m_Stats.GetStat(STAT_WARP_RECOVERY)); //TODO: Get warp reset time from ship index
	    ShipIndex()->SetEngineThrustState(0);
        SendAuxShip();

		//check arrival nav for talk tree mission
		ObjectManager *om = GetObjectManager();
		if (om)
		{
			Object *obj = om->GetObjectFromID(ShipIndex()->GetTargetGameID());
			if (obj && obj->GetUsedInMission() && obj->RangeFrom(Position()) <= 5000.0f)
			{
				CheckMissions(obj->GetDatabaseUID(), 1, obj->GetDatabaseUID(), TALK_SPACE_NPC);
			}
		}

        if (player_forced && m_WarpTime > 0)
        {
            m_WarpTime = 0;
        }

        if (m_SectorMgr)
        {
            m_SectorMgr->AddTimedCall(this, B_WARP_RESET, (u32)m_Stats.GetStat(STAT_WARP_RECOVERY), NULL); //TODO: Get warp reset time from ship index
        }
        RechargeReactor();
        CheckNavs();
        UpdateVerbs();
        m_SetUpdate = 1;
        m_WarpDrive = false;
    }

    SavePosition();
}

void Player::FinalWarpReset()
{
    ShipIndex()->SetPrivateWarpState(PRIVATE_WARP_AVAILABLE); 
	SendAuxShip();
}

float Player::GetEnergy()
{
	u32 myTime = GetNet7TickCount();

	if (m_LastReactorChange == 0)
	{
		m_LastReactorChange = myTime;
	}

	// Dont update when dead
	if (ShipIndex()->GetIsIncapacitated())
	{
		m_LastReactorChange = myTime;
	}

    u32 timeElapsed = myTime - m_LastReactorChange;

	if (myTime > ShipIndex()->Energy.GetEndTime())
	{
		timeElapsed = ShipIndex()->Energy.GetEndTime() - m_LastReactorChange;
	}

	float myEnergy = ShipIndex()->Energy.GetStartValue() + (float)(timeElapsed) * ShipIndex()->Energy.GetChangePerTick();

	if (myEnergy > 1.0f)
	{
		myEnergy = 1.0f;
	}
	else if (myEnergy < 0.0f)
	{
		myEnergy = 0.0f;
	}

	//LogMessage("Energy: %f\n",myEnergy);

	return (myEnergy);
}

float Player::GetEnergyValue()
{
	return ((GetEnergy() * ShipIndex()->GetMaxEnergy()));
}

float Player::GetShield()
{
	u32 myTime = GetNet7TickCount();

	if (m_LastShieldChange == 0)
	{
		m_LastShieldChange = myTime;
	}

	u32 timeElapsed = myTime - m_LastShieldChange;

	if (myTime > ShipIndex()->Shield.GetEndTime())
	{
		timeElapsed = ShipIndex()->Shield.GetEndTime() - m_LastShieldChange;
	}

	float myShield = ShipIndex()->Shield.GetStartValue() + (float)(timeElapsed) * ShipIndex()->Shield.GetChangePerTick();

	if (myShield > 1.0f)
	{
		myShield = 1.0f;
	}
	else if (myShield < 0.0f)
	{
		myShield = 0.0f;
	}

	//LogMessage("Shield: %f\n",myShield);

	return (myShield);
}

float Player::GetShieldValue()
{
	return ((GetShield() * ShipIndex()->GetMaxShield()));
}

void Player::DrainReactor(unsigned long DrainTime, float EnergyUsed)
{
	float myEnergy = GetEnergy();

	if (DrainTime > 0) 
	{
		float drainRate = ((EnergyUsed/(float)DrainTime) / ShipIndex()->GetMaxEnergy());
		EnergyUpdate(GetNet7TickCount() + (unsigned long)(myEnergy / drainRate), -drainRate, myEnergy);
	}
	else
	{
		LogDebug("Drain Reactor used with Draintime = 0\n");
		RemoveEnergy(EnergyUsed);
	}
}

void Player::RemoveEnergy(float EnergyRemoved)
{
	unsigned long endTime;
	float myEnergy = GetEnergy() - (EnergyRemoved / ShipIndex()->GetMaxEnergy());

	if (ShipIndex()->Energy.GetChangePerTick() > 0.0f)
	{
		endTime = (unsigned long)((1.0f - myEnergy) / ShipIndex()->Energy.GetChangePerTick());
	}
	else
	{
		endTime = (unsigned long)(myEnergy / ShipIndex()->Energy.GetChangePerTick());
	}
	
	// Dont regen if Incapacited
	if (ShipIndex()->GetIsIncapacitated())
    {
		// Stop Regen
		EnergyUpdate(0, 0, myEnergy);
	}
	else
	{
		// Start regen if we are revived
		if (BaseEnergyRecharge() == 0)
		{
			ShipIndex()->Energy.SetChangePerTick(BaseEnergyRecharge());
		}
		EnergyUpdate(GetNet7TickCount() + endTime, ShipIndex()->Energy.GetChangePerTick(), myEnergy);
	}
}

//DB Shield Recharge stops
void Player::RemoveShield(float ShieldRemoved)
{
    unsigned long CurTick = GetNet7TickCount();
	float myShield = GetShield() - (ShieldRemoved / ShipIndex()->GetMaxShield());

    if (myShield < 0.0f)
    {
        myShield = 0;
    }

    if (ShipIndex()->Shield.GetChangePerTick() != 0)
    {
        ShipIndex()->Shield.SetEndTime(0);
        ShipIndex()->Shield.SetChangePerTick(0);
    }

	ShipIndex()->Shield.SetStartValue(myShield);

    m_LastShieldChange = GetNet7TickCount();
    SendAuxShip();

    //set a 10 second recharge delay
    static unsigned long RechargeDelay = 10000;

    if (m_ShieldRecharge > 0)
    {
		if (m_ShieldRecharge < (CurTick + RechargeDelay))
		{
			m_ShieldRecharge = CurTick + RechargeDelay;
		}
    }
    else
    {
        m_ShieldRecharge = CurTick + RechargeDelay;
    }
}
void Player::EnergyUpdate(unsigned long EndTime, float ChangePerTick, float StartValue)
{
	//LogMessage("EnergyUpdate - CurTime: %x EndTime: %x, Change: %f, Start: %f\n", GetNet7TickCount(), EndTime, ChangePerTick, StartValue);
    m_LastReactorChange = GetNet7TickCount();
    ShipIndex()->Energy.SetEndTime(EndTime);
    ShipIndex()->Energy.SetChangePerTick(ChangePerTick);
    ShipIndex()->Energy.SetStartValue(StartValue);

    SendAuxShip();
}

void Player::ShieldUpdate(unsigned long EndTime, float ChangePerTick, float StartValue)
{
	if (ShipIndex()->GetIsIncapacitated()) ChangePerTick = 0.0f;

    m_LastShieldChange = GetNet7TickCount();
    ShipIndex()->Shield.SetEndTime(EndTime);
    ShipIndex()->Shield.SetChangePerTick(ChangePerTick);
    ShipIndex()->Shield.SetStartValue(StartValue);

    SendAuxShip();
}

void Player::RechargeReactor()
{
	float Energy = GetEnergy();
	unsigned long RequiredTime = (unsigned long)((1.0f - Energy) / BaseEnergyRecharge());
	
	// Dont regen if Incapacited
	if (!ShipIndex()->GetIsIncapacitated())
	{
		EnergyUpdate(GetNet7TickCount() + RequiredTime, BaseEnergyRecharge(), Energy);
		m_EnergyRecharge = 0;
	}
}

void Player::RechargeShield()
{
    float Shield = GetShield();
	unsigned long RequiredTime = (unsigned long)((1.0f - Shield) / BaseShieldRecharge());

	// Dont regen if Incapacited
	if (!ShipIndex()->GetIsIncapacitated())
	{
		m_ShieldRecharge = 0;
		ShieldUpdate(GetNet7TickCount() + RequiredTime, BaseShieldRecharge(), Shield);
	}
}

//use this method to halt various activities. There's lots that doesn't need to go into the time slot system
//devices can go onto the normal time queue because usage is not so intense.
void Player::CheckEventTimes(unsigned long current_tick)
{
	if (PlayerIndex()->GetSectorNum() > 9999)
	{
		//station events go here
		if (m_BroadcastPositionTick > 0 && m_BroadcastPositionTick < current_tick)
		{
			BroadcastPosition();
			m_BroadcastPositionTick = 0;
		}
	}
	else
	{
		//space events go here
		// Dont regen if Incapacited
		if (ShipIndex()->GetIsIncapacitated())
		{
			return;
		}

		if (m_ShieldRecharge > 0 && m_ShieldRecharge < current_tick)
		{
			RechargeShield();
		}

		if (m_EnergyRecharge > 0 && m_EnergyRecharge < current_tick)
		{
			RechargeReactor();
		}

		if (m_ProspectBeamNode.player_id !=0 && m_ProspectBeamNode.event_time < current_tick)
		{
			m_ProspectBeamNode.player_id = 0;
			PullOreFromResource(m_ProspectBeamNode.obj, m_ProspectBeamNode.i1, m_ProspectBeamNode.i2, m_ProspectBeamNode.i3, m_ProspectBeamNode.i4);
		}

		if (m_ProspectTractorNode.player_id !=0 && m_ProspectTractorNode.event_time < current_tick)
		{
			m_ProspectTractorNode.player_id = 0;
			TakeOreOnboard(m_ProspectTractorNode.obj, m_ProspectTractorNode.i3);
		}

		if (m_WarpBroadcastTime > 0 && m_WarpBroadcastTime < current_tick)
		{
			StartWarp();
		}
	}
}

///////////////////////////////////////////////////
// Range List Handling
//
// These methods handle adding to and removing from
// the player range lists.

void Player::UpdatePlayerVisibilityList()
{
    Player *p = 0;
    u32 * sector_list = GetSectorPlayerList();
    bool in_range;
    float player_range;
    bool group_member;
	bool DistressBeacon;

    if (!InSpace() || (!m_SectorMgr)) return;

    //TODO: 
    //      - Only send the player info once (use bitfield to indicate playerinfo sent).
    //      - Add grid system to further reduce checking

	while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
	{
		if (p != this)
		{
            //get range and find out if this player is already on the list
            player_range = p->RangeFrom(Position());
            in_range = PlayerInRangeList(p);

			group_member = g_ServerMgr->m_PlayerMgr.CheckGrouped(this, p);
			DistressBeacon = ShipIndex()->GetIsRescueBeaconActive();

            if (in_range) 
            { 
                //see if we've gone out of their scan range
                if ((!DistressBeacon && !group_member) && (player_range > (float)(p->ShipIndex()->CurrentStats.GetScanRange() + ShipIndex()->CurrentStats.GetVisibility()) + 200.0f)) //200.0f stops 'ratcheting'
                {                   
					RemovePlayerFromRangeList(p);
					m_Effects.SendRemoveEffects(p);
                }
            }
            else
            {
                //see if we've come into their scan range
                if (DistressBeacon || group_member || player_range < (float)(p->ShipIndex()->CurrentStats.GetScanRange() + ShipIndex()->CurrentStats.GetVisibility()) && p->InSpace())
                {
                    AddPlayerToRangeList(p);
					m_Effects.SendEffects(p);
                }
            }
		}
	}
}

bool Player::PlayerInRangeList(Player *p_check)
{
	return (g_PlayerMgr->GetIndex(p_check, m_RangeList));
}

//this means that player 'p' can no longer see what this player does
void Player::RemovePlayerFromRangeList(Player *p)
{
	if (g_PlayerMgr->GetIndex(p, m_RangeList))
	{
		g_PlayerMgr->UnSetIndex(p, m_RangeList);
		g_PlayerMgr->UnSetIndex(p, m_ClickedList);
		if (!p->DebugPlayer())
		{
			p->RemoveObject(GameID());
			if (g_Debug && p->AdminLevel() == 90)
			{
				p->SendVaMessage("%s just gone out of your scan range.", Name());
			}
		}
		else if (g_Debug && p->AdminLevel() == 90)
		{
			SendVaMessage("We've just gone out of scan range of %s.", p->Name());
		}
	}
}

//This means that player 'p' can now see anything that this player does.
void Player::AddPlayerToRangeList(Player *p)
{
	g_PlayerMgr->SetIndex(p, m_RangeList);
	g_PlayerMgr->UnSetIndex(p, m_ClickedList);

    if (!p->DebugPlayer() && p->GameID() != GameID())
    {
        if (g_Debug && p->AdminLevel() == 90)
        {
            p->SendVaMessage("%s just came into your scan range.", Name());
        }
        SendShipData(p); //send our ship to 'p' - we can cut this down by separating out some data into the 'OnTargetted' method
    }
    else if (g_Debug && p->AdminLevel() == 90 && !DebugPlayer())
    {
        SendVaMessage("We just came into scan range of %s.", p->Name());
    }
}

//Send object to all players who can see us
void Player::SendToVisibilityList(bool include_player)
{
	Player *p = (0);

	if (m_SendDockPos && !m_SentDockPos)
	{
		m_SentDockPos = true;
	}
	else if (m_SentDockPos)
	{
		return; //don't update position
	}

	//sort out 'this' player first if we need to
	if (include_player)
	{
		if (!g_ServerMgr->m_PlayerMgr.CheckGroupFormation(this))
		{
			SendAdvancedPositionalUpdate(GameID(), &m_Position_info);
		}
		else if (g_ServerMgr->m_PlayerMgr.CheckGroupFormation(this))
		{
			// Send formation info
			g_ServerMgr->m_PlayerMgr.SendFormation(this, this);
		}
	}

    long update = m_Position_info.UpdatePeriod; //this could smooth out player movement a little.
    m_Position_info.UpdatePeriod = 500;

	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))
	{
		if (!IsInSameSector(p)) 
		{
			g_PlayerMgr->UnSetIndex(p, m_RangeList);
		}
		else if (p != this)
		{
			if (m_ReceivedMovement && p->Active() && !p->DebugPlayer() && !g_ServerMgr->m_PlayerMgr.CheckGroupFormation(p))
			{
				p->SendAdvancedPositionalUpdate(GameID(), &m_Position_info);
			} 
			else if (g_ServerMgr->m_PlayerMgr.CheckGroupFormation(p))
			{
				// Send formation info
				g_ServerMgr->m_PlayerMgr.SendFormation(this, p);
			}	
		}
	}

    m_Position_info.UpdatePeriod = update;

    m_ReceivedMovement = false;
}

//this returns true if Player 'player' has clicked on 'this' player
bool Player::IsClickedBy(Player *player)
{
	return g_PlayerMgr->GetIndex(player, m_ClickedList);
}

void Player::SetClickedBy(Player *p)
{
	g_PlayerMgr->SetIndex(p, m_ClickedList);
}

void Player::RemoveFromAllSectorRangeLists()
{
    RangeListVec::iterator itrRList;
    Player *p;
    long sector_id = PlayerIndex()->GetSectorNum();

    if (sector_id == 0)
    {
        return;
    }

    u32 * sector_list = GetSectorPlayerList();
	SectorManager *sect_manager = g_ServerMgr->GetSectorManager(sector_id);
    ObjectManager *obj_manager = GetObjectManager();

    if (m_PlayerIndex.GetSectorNum() <= 9999) //in space
    {
		p = (0);
		// Send the Remove packet to everyone that can see us
		while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))
		{
            if (p->Active())
            {
                p->RemoveObject(GameID());
            }
        }

        if (sector_list)
        {
			p = (0);
            while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
			{
                if (p != this)
                {
                    p->RemovePlayerFromRangeList(this);
                }
            }
        }
        
        if (obj_manager)
        {
            obj_manager->RemovePlayerFromMOBRangeLists(this);
        }
    }
    else
    {
        m_Oldroom = m_Room;
        m_Room = -1;
        SendStarbaseAvatarList();
    }

    //now remove from the sector list itself
    if (sect_manager) sect_manager->RemovePlayerFromSectorList(this);

	SaveHullLevelChange(ShipIndex()->GetMaxHullPoints());
	SaveEnergyLevels();
}

////////////////////////////////////////////////////
//
// These methods traverse the player visibility lists
// And send FX packets to all ships that can see us
// TODO: most of these could be bunched into one call

void Player::SendTractorComponentRL(Object *obj, float decay, float tractor_speed, long article_id, long effect_id, long timestamp)
{
	m_Mutex.Lock();
	
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
        p->CreateTractorComponent(obj->Position(), decay, tractor_speed, GameID(), article_id, effect_id, timestamp);
	}

	m_Mutex.Unlock();
}

void Player::SendResourceNameRL(long article_UID, char *raw_name)
{
	m_Mutex.Lock();
	
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
        p->SendResourceName(article_UID, raw_name);
    }

	m_Mutex.Unlock();
}

void Player::SendResourceRemoveRL(long article_UID)
{
	m_Mutex.Lock();
	
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
        p->RemoveObject(article_UID);
	}

	m_Mutex.Unlock();
}

void Player::SendContrailsRL(bool contrails)
{
	m_Mutex.Lock();
	
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
        p->Contrails(GameID(), contrails);
	}

	m_Mutex.Unlock();
}

void Player::SendEffectRL(long target_id, char *message, long effect_type, long effectUID, long timestamp, short duration)
{
	m_Mutex.Lock();
	
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
        p->ActivateProspectBeam(GameID(), target_id, message, (short)effect_type, effectUID, timestamp, duration);
	}

	m_Mutex.Unlock();
}

void Player::RemoveEffectRL(long effect_UID)
{
	m_Mutex.Lock();
	
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
        p->SendRemoveEffect(effect_UID);
	}

	m_Mutex.Unlock();	
}

void Player::SendObjectEffectRL(ObjectEffect *obj_effect)
{
	unsigned char effect[128];
	memset(effect,0,128);
    int index = 0;

    AddData(effect, obj_effect->Bitmask, index);
    AddData(effect, obj_effect->GameID, index);
    AddData(effect, obj_effect->EffectDescID, index);

	if (obj_effect->Bitmask & 0x01)
	{
        AddData(effect, obj_effect->EffectID, index);
	}
	if (obj_effect->Bitmask & 0x02)
	{
        if (obj_effect->TimeStamp == 0)
        {
            obj_effect->TimeStamp = GetNet7TickCount();
        }

        AddData(effect, obj_effect->TimeStamp, index);
	}
	if (obj_effect->Bitmask & 0x04)
	{
        AddData(effect, obj_effect->Duration, index);
	}
	if (obj_effect->Bitmask & 0x08)
	{
        AddData(effect, obj_effect->Scale, index);
	}
	if (obj_effect->Bitmask & 0x10)
	{
        AddData(effect, obj_effect->HSVShift[0], index);
	}
	if (obj_effect->Bitmask & 0x20)
	{
        AddData(effect, obj_effect->HSVShift[1], index);
	}
	if (obj_effect->Bitmask & 0x40)
	{
        AddData(effect, obj_effect->HSVShift[2], index);
	}

    SendToRangeList(ENB_OPCODE_0009_OBJECT_EFFECT, effect, index);
}

//Sometimes, we might need to send an effect to a specific player only
void Player::SendEffect(ObjectEffect *obj_effect)
{
	unsigned char effect[128];
	memset(effect,0,128);
    int index = 0;

    AddData(effect, obj_effect->Bitmask, index);
    AddData(effect, obj_effect->GameID, index);
    AddData(effect, obj_effect->EffectDescID, index);

	if (obj_effect->Bitmask & 0x01)
	{
        AddData(effect, obj_effect->EffectID, index);
	}
	if (obj_effect->Bitmask & 0x02)
	{
        if (obj_effect->TimeStamp == 0)
        {
            obj_effect->TimeStamp = GetNet7TickCount();
        }

        AddData(effect, obj_effect->TimeStamp, index);
	}
	if (obj_effect->Bitmask & 0x04)
	{
        AddData(effect, obj_effect->Duration, index);
	}
	if (obj_effect->Bitmask & 0x08)
	{
        AddData(effect, obj_effect->Scale, index);
	}
	if (obj_effect->Bitmask & 0x10)
	{
        AddData(effect, obj_effect->HSVShift[0], index);
	}
	if (obj_effect->Bitmask & 0x20)
	{
        AddData(effect, obj_effect->HSVShift[1], index);
	}
	if (obj_effect->Bitmask & 0x40)
	{
        AddData(effect, obj_effect->HSVShift[2], index);
	}

	SendOpcode(ENB_OPCODE_0009_OBJECT_EFFECT, effect, index);
}

void Player::SendObjectToObjectEffectRL(ObjectToObjectEffect *obj_effect, bool weapon_fire)
{
	unsigned char effect[128];
	memset(effect,0,128);
    int index = 0;

    AddData(effect, obj_effect->Bitmask, index);
    AddData(effect, obj_effect->GameID, index);
    AddData(effect, obj_effect->TargetID, index);
    AddData(effect, obj_effect->EffectDescID, index);

    if (obj_effect->Message)
    {
        AddDataS(effect, obj_effect->Message, index);
    }
    
    AddData(effect, (char)0, index);

	if (obj_effect->Bitmask & 0x01)
	{
        AddData(effect, obj_effect->EffectID, index);
	}
	if (obj_effect->Bitmask & 0x02)
	{
        if (obj_effect->TimeStamp == 0)
        {
            obj_effect->TimeStamp = GetNet7TickCount();
        }

        AddData(effect, obj_effect->TimeStamp, index);
	}
	if (obj_effect->Bitmask & 0x04)
	{
        AddData(effect, obj_effect->Duration, index);
	}
	if (obj_effect->Bitmask & 0x08)
	{
        AddData(effect, obj_effect->TargetOffset[0], index);
        AddData(effect, obj_effect->TargetOffset[1], index);
        AddData(effect, obj_effect->TargetOffset[2], index);
	}
	if (obj_effect->Bitmask & 0x10)
	{
        AddData(effect, obj_effect->OutsideTargetRadius, index);
	}
	if (obj_effect->Bitmask & 0x20) //from here on isn't correct - packet struct is wrong.
	{
        AddData(effect, obj_effect->unused, index);
	}
    if (obj_effect->Bitmask & 0x40)
    {
        AddData(effect, obj_effect->Scale, index);
    }
	if (obj_effect->Bitmask & 0x80)
	{
        AddData(effect, obj_effect->HSVShift[0], index);
        AddData(effect, obj_effect->HSVShift[1], index);
        AddData(effect, obj_effect->HSVShift[2], index);
	}
    if (obj_effect->Bitmask & 0x100)
    {
        AddData(effect, obj_effect->Speedup, index);
    }

    SendToRangeList(ENB_OPCODE_000B_OBJECT_TO_OBJECT_EFFECT, effect, index, weapon_fire);
}

void Player::SendToRangeList(short opcode, unsigned char *data, size_t length, bool weapon_fire)
{
	m_Mutex.Lock();
	
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
		if (weapon_fire)
		{
			long player_id = *((long*) &data[2]);
			if (m_WeaponsPerTick < MAX_WEAPON_FIRE_PER_TICK || player_id == this->GameID()) //always ensure player's own weapons are shown
			{
				p->SendOpcode(opcode, data, length);
				m_WeaponsPerTick++;
			}
		}
		else
		{
			p->SendOpcode(opcode, data, length);
		}
	}

    m_Mutex.Unlock();
}

void Player::SendToGroup(short opcode, unsigned char *data, size_t length)
{
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
        if (p == this || g_ServerMgr->m_PlayerMgr.CheckGrouped(this, p) )
        {
            p->SendOpcode(opcode, data, length);
        }
	}
}

void Player::SendToSector(short opcode, unsigned char *data, size_t length) //this method is a dumb send to everyone in the sector
{
    if (!m_SectorMgr) return;

    u32 * sector_list = GetSectorPlayerList();
	Player *p = (0);
	
    while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
	{
		p->SendOpcode(opcode, data, length);
    }
}

void Player::SendObjectCreateRL(long article_UID, float scale, short basset, int type)
{
	m_Mutex.Lock();
	
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
        p->SendCreate(article_UID, scale, basset, type, 0.0f, 0.0f, 0.0f);			
	}
	m_Mutex.Unlock();
}

void Player::SendRelationshipRL(long article_UID, long relationship, long is_attacking)
{
	bool b_is_attacking = (is_attacking == 1) ? true : false;
	
	m_Mutex.Lock();
	
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
        p->SendRelationship(article_UID, relationship, b_is_attacking);
	}
	m_Mutex.Unlock();
}

int LevelXP[] = { 500,1000,1500,2000,3000,4000,5000,7000,9000,11000,13000,15000,20000,25000,
				  30000,35000,40000,47500,55000,62500,70000,77500,87500,97500,107500,117500,127500,
				  140000,152500,165000,177500,190000,205000,220000,235000,250000,265000,285000,305000,
				  325000,345000,365000,390000,415000,440000,475000,510000,545000,595000,645000,
				  // Added XP for levels over 50
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000
				};


void Player::AwardNavExploreXP(Object *obj)
{
    char message[128];
    long xp = 250;
    sprintf(message, "Discovered %s", obj->Name());
    SendClientSound("Location_Discovered", 0, 0);
    
    //this is just a hack for now - hidden objs will give 500xp, small hidden objs give 1000xp.
    if (obj->Signature() < 5000.0f) xp *= 2;
    if (obj->Signature() < 1500.0f) xp *= 2;
    if (obj->ObjectType() == OT_STARGATE) xp = 1000;
   
    AwardExploreXP(message, xp);
	SaveExploreNav(obj->GetDatabaseUID());
}

float Player::AwardXP(experience_type xp_type, long xp_gain, bool skip_group)
{
    u32 level;
    u32 skill_points_earned = 0;
    long xp;
    char xp_string[16];
    char msg_buffer[128];

    switch (xp_type)
    {
    case XP_COMBAT:
        level = PlayerIndex()->RPGInfo.GetCombatLevel();
        xp = (u32)(PlayerIndex()->RPGInfo.GetCombatXP() * LevelXP[level]) + xp_gain;
        sprintf(xp_string, "Combat");
        break;

    case XP_EXPLORE:
        level = PlayerIndex()->RPGInfo.GetExploreLevel();
        xp = (u32)(PlayerIndex()->RPGInfo.GetExploreXP() * LevelXP[level]) + xp_gain;
        sprintf(xp_string, "Explore");
        break;

    case XP_TRADE:
        level = PlayerIndex()->RPGInfo.GetTradeLevel();
        xp = (u32)(PlayerIndex()->RPGInfo.GetTradeXP() * LevelXP[level]) + xp_gain;
        sprintf(xp_string, "Trade");
        break;

    default:
        LogMessage("Bad XP type for player %s\n",Name());
        return 0;
        break;
    }
    
    if (level > 50)
    {
        level = 50;
    }
   
    while (xp >= LevelXP[level]) 
    {			
        xp -= LevelXP[level];
        if (level <= 19) 
        {
            skill_points_earned++;
            level++;
        } 
        else if (level <= 39) 
        {
            skill_points_earned += 2;
            level++;
        } 
        else if (level < 50) 
        {
            skill_points_earned += 3;
            level++;
        } 
        else
        {
            skill_points_earned++;
            //TODO: distribute XP to other bars
        }
    }
    
    float xp_bar = (float)(xp)/(float)(LevelXP[level]);
       
    if (skill_points_earned > 0)
    {
        SendClientSound("Player_Levels",0,1);
        
        switch (xp_type)
        {
        case XP_COMBAT:
            PlayerIndex()->RPGInfo.SetCombatLevel(level);
            ShipIndex()->SetCombatLevel(level);
            break;

        case XP_EXPLORE:
            PlayerIndex()->RPGInfo.SetExploreLevel(level);
            break;
        
        case XP_TRADE:
            PlayerIndex()->RPGInfo.SetTradeLevel(level);
            break;
        }

        PlayerIndex()->RPGInfo.SetSkillPoints(PlayerIndex()->RPGInfo.GetSkillPoints() + skill_points_earned);
        PlayerIndex()->RPGInfo.SetTotalSkillPoints(PlayerIndex()->RPGInfo.GetTotalSkillPoints() + skill_points_earned);

		SaveSkillPoints();
        
        SendPushMessage("LEVEL UP!","QuickLine",0,3);
        sprintf(msg_buffer, "%s level is now %d!", xp_string, level);
        SendPushMessage(msg_buffer, "MessageLine", 3000, 3);
       
        // Update the level display when targeting
        LevelUpForSkills();
        UpdateSkills();       
        SendAuxShip();
		SaveAdvanceLevel(xp_type, level);
    }

	SaveXPBarLevel(xp_type, xp_bar);

    return xp_bar;
}

void Player::AwardExploreXP(char *message, long xp_gain, bool skip_group)
{
    bool GroupXP = false;
    char msg_buffer[128];

#ifdef BETA_TESTING
    xp_gain *= 4;
#endif

    //TODO: Tidy up group XP code
	if (!skip_group)
	{
		GroupXP = g_ServerMgr->m_PlayerMgr.GroupXP(this, message, xp_gain, GroupID(), 1);		// 0 = Combat 1 = Explore
	    sprintf(msg_buffer, "%s %d Explore experience earned", message, xp_gain);
		SendPriorityMessageString(msg_buffer,"MessageLine",5000,4);
	} 

	if (!GroupXP)
	{
        float xp_bar = AwardXP(XP_EXPLORE, xp_gain, skip_group);
        PlayerIndex()->RPGInfo.SetExploreXP(xp_bar);
        SendAuxPlayer();
    }
}

/*05% till -10, 0 xp below that
-5 05%
-4 10%
-3 25%
-2 50%
-1 80%
0 100%
+1 120%
+2 150%
+3 175%
+4 200%
+5 300%
+6 400%
+7 500%
+8 800%

* We are introducing a level band for combat experience based on the highest combat level (CL) in the group. 
The formula for this is ((Highest CL / 10) + 5). A CL 50 player can group with people down to CL40, 
and everyone in the group would get XP as they do now. So, if 2 CL50's, a CL45, a CL40,
and a CL30 were grouped together, the 2 50's, the 45 and the 40 would receive experience as normal. 
The CL30 would be outside of the level band, and would receive much lower combat XP than they do now.*/

short Player::CalculateMOBXP(short mob_level)
{
    short xp_earned;
    float multiplier = 0.1f;
    short level_diff = mob_level - PlayerIndex()->RPGInfo.GetCombatLevel();

    switch (level_diff)
    {
    case -4:
        multiplier = 0.1f;
        break;

    case -3:
        multiplier = 0.25f;
        break;

    case -2:
        multiplier = 0.50f;
        break;

    case -1:
        multiplier = 0.80f;
        break;

    case 0:
        multiplier = 1.0f;
        break;

    case 1:
        multiplier = 1.20f;
        break;

    case 2:
        multiplier = 1.50f;
        break;

    case 3:
        multiplier = 1.75f;
        break;

    case 4:
        multiplier = 2.00f;
        break;

    case 5:
        multiplier = 3.00f;
        break;

    case 6:
        multiplier = 4.00f;
        break;

    case 7:
        multiplier = 5.00f;
        break;

    case 8:
        multiplier = 8.00f;
        break;

    default:
        if (level_diff < -10) multiplier = 0.0f;
        if (level_diff > 8) multiplier = 8.00f;
    }

    xp_earned = (short) (1000.0f * multiplier);

    return xp_earned;
}

void Player::AddMOBDestroyExperience(short mob_level, char *mob_name)
{
	char msg_buffer[128];

    //if player is grouped, this will not display the correct XP, but it's only for debug.
	short xp_earned = CalculateMOBXP(mob_level);
	LogMessage("Mob XP: %d MobLevel: %d (%s)\n", xp_earned, mob_level, Name());
	
	_snprintf(msg_buffer, 128, "Defeated %s:", mob_name);

    g_ServerMgr->m_PlayerMgr.GroupCombatXP(this, msg_buffer, mob_level);
}

void Player::AwardCombatXP(char *message, long xp_gain, bool group)
{
    bool GroupXP = false;
    char msg_buffer[128];

    if (xp_gain > 8000) xp_gain = 8000;

    if (group)
    {
        _snprintf(msg_buffer, 128, "%s", message); 
    }
    else
    {	
        _snprintf(msg_buffer, 128, "%s %d Combat experience earned", message, xp_gain); 
    }
   
	SendPriorityMessageString(msg_buffer,"MessageLine",5000,4);

    float xp_bar = AwardXP(XP_COMBAT, xp_gain, group);
    PlayerIndex()->RPGInfo.SetCombatXP(xp_bar);
    SendAuxPlayer();
}

void Player::AwardTradeXP(char *message, long xp, bool skip_group)
{
    char msg_buffer[256];

	//TO-DO: Players are complaining trade XP is too low. Increase tradeXP for selling or
	//  put a multiplier for tradeXP in here.

#ifdef BETA_TESTING
    xp *= 4;
#endif

    _snprintf(msg_buffer, 256, "%s %d Trade experience earned", message, xp); 
    SendPriorityMessageString(msg_buffer,"MessageLine",5000,4);

    float xp_bar = AwardXP(XP_TRADE, xp);
    PlayerIndex()->RPGInfo.SetTradeXP(xp_bar);
    SendAuxPlayer();
}

// put this back on the stack again.
// Server should avoid new/delete cycles if it wants to stay working
// We can now allocate a persistant heap space for these messages.
void Player::SendVaMessage(char *string, ...)
{
    unsigned int len = strlen(string) + 256;
    char *pch = (char*)_alloca(len);
    
    va_list args;
    va_start(args, string);
    vsprintf(pch, string, args);
    SendMessageString(pch);
    va_end(args);
}

//Variant of SendMessage which lets you use a colour

//17 - red
//13 - white
//12 - light green
//11 - dark blue
void Player::SendVaMessageC(char colour, char *string, ...)
{
    unsigned int len = strlen(string) + 256;
    char *pch = (char*)_alloca(len);
    
    va_list args;
    va_start(args, string);
    vsprintf(pch, string, args);
    SendMessageString(pch, colour);
    va_end(args);
}

long Player::TractorRange()
{
    return (1000 + PlayerIndex()->RPGInfo.GetHullUpgradeLevel() * 250);
}

long Player::ProspectRange()
{
    return (750 + (PlayerIndex()->RPGInfo.Skills.Skill[SKILL_PROSPECT].GetLevel() * 250));
}

void Player::BlankVerbs()
{
    memset(m_Verbs, 0, sizeof(m_Verbs));
}

bool Player::AddVerb(short verb_id, float verb_activate_range)
{
    bool success = false;
    for (int i = 0; i < 4; i++)
    {
        if (m_Verbs[i].verb_id == 0)
        {
            m_Verbs[i].verb_id = verb_id;
            m_Verbs[i].activate_range = verb_activate_range;
            m_Verbs[i].active = false;
            success = true;
            break;
        }
    }

    return success;
}

void Player::UpdateVerbs(bool force_update)
{
	unsigned char verb_update[100];
	int index = 0;
	long drops = 0;
    bool changed = force_update;
    short default_attribute = ATTRIBUTE_DIS_TOOFAR;
    short attribute;
	int i;

	ObjectManager *om = GetObjectManager();

    Object *obj = (0);
	
	if (om) obj = om->GetObjectFromID(ShipIndex()->GetTargetGameID());

    if (!obj)
    {
        return;
    }

    float target_range = obj->RangeFrom(Position());

    for (i = 0; i < 4; i++)
    {
        if (m_Verbs[i].verb_id > 0)
        {
            drops++;
            if ((target_range <= m_Verbs[i].activate_range || m_Verbs[i].activate_range == 0.0f) &&
                m_Verbs[i].activate_range != -1.0f) //in_range
            {
                if (!m_Verbs[i].active)
                {
                    m_Verbs[i].active = true;
                    changed = true;
                }
            }
            else
            {
                if (m_Verbs[i].active)
                {
                    m_Verbs[i].active = false;
                    changed = true;
                    CloseInterfaceIfRequired(m_Verbs[i].verb_id);
                }
            }
        }
    }

    if (changed)
    {
        AddData(verb_update, ntohl(obj->GameID()), index);
        AddData(verb_update, ntohl(drops), index);

        for (i = 0; i < 4; i++)
        {
            if (m_Verbs[i].verb_id > 0)
            {
                AddData(verb_update, default_attribute, index);
                AddData(verb_update, m_Verbs[i].verb_id, index);
            }
        }

        AddData(verb_update, ntohl(drops), index);

        for (i = 0; i < 4; i++)
        {
            if (m_Verbs[i].verb_id > 0)
            {
                attribute = m_Verbs[i].active ? ATTRIBUTE_ENABLE : ATTRIBUTE_DIS_TOOFAR;
                AddData(verb_update, attribute, index);
                AddData(verb_update, m_Verbs[i].verb_id, index);
            }
        }
        
        SendOpcode(ENB_OPCODE_005C_VERB_UPDATE, verb_update, index);
    }
}

void Player::CloseInterfaceIfRequired(short verb_id)
{
    switch (verb_id)
    {
    case VERBID_LOOT:
    case VERBID_PROSPECT:
        CloseInterfaceIfOpen();
        break;
    case VERBID_TRADE: //todo: close trade window
        break;
    }
}

//Player 'player' is targetting this player
void Player::OnTargeted(Player *player)
{
    if (player != this)
    {
        player->BlankVerbs();
        if (ShipIndex()->GetIsIncapacitated()) //if the targetted player is incapacitated
        {
            player->AddVerb(VERBID_TRADE, -1.0f);
            player->AddVerb(VERBID_GROUP, -1.0f);
            if (player->Database()->ship_data.profession == PROFESSION_EXPLORER &&
                player->m_PlayerIndex.RPGInfo.Skills.Skill[SKILL_JUMPSTART].GetLevel() > 0 )
            {
				// add in range as skill goes up
				int Range = (player->m_PlayerIndex.RPGInfo.Skills.Skill[SKILL_JUMPSTART].GetLevel() - 1) * 250;
                player->AddVerb(VERBID_JUMPSTART, 1000.0f + Range);
            }
        }
        else if (player->ShipIndex()->GetIsIncapacitated()) //player doing the clicking is incapacitated
        {
            player->AddVerb(VERBID_TRADE, -1.0f);
            player->AddVerb(VERBID_GROUP, -1.0f);
            player->AddVerb(VERBID_FOLLOW, -1.0f);
        }
        else
        {
            player->AddVerb(VERBID_TRADE, 1000.0f);
            player->AddVerb(VERBID_GROUP, 0.0f);
            player->AddVerb(VERBID_FOLLOW, 1500.0f);
        }

        player->AddVerb(VERBID_MESSAGE, 0.0f);

        if (!IsClickedBy(player))
        {
            SetClickedBy(player);

            m_ShipIndex.Buffer()->Lock();

            if (m_ShipIndex.BuildClickExtendedPacket())
            {
                player->SendOpcode(ENB_OPCODE_001B_AUX_DATA, 
                    m_ShipIndex.m_Packet, 
                    m_ShipIndex.m_PacketLength);
            }

            m_ShipIndex.Buffer()->Unlock();
        }
    } 
    else 
    { 
		// We can Jumpstart ourselves
		if (Database()->ship_data.profession == PROFESSION_EXPLORER &&
			player->m_PlayerIndex.RPGInfo.Skills.Skill[SKILL_JUMPSTART].GetLevel() > 0 &&
			ShipIndex()->GetIsIncapacitated())
		{
			BlankVerbs();
			AddVerb(VERBID_JUMPSTART, 1000.0f);
		}
	}
}

void Player::SetDistressBeacon(bool Status)
{
	ShipIndex()->SetIsRescueBeaconActive(Status);
	//BlankRangeList();
	//all ships in sector will now be sent your ship with distress beacon toggled on
}

void Player::BlankRangeList()
{
	m_Mutex.Lock();
		
	Player *p = (0);
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))	
	{
		if (p->GameID() != GameID())
		{
			p->RemoveObject(GameID());
		}
	}

	AddPlayerToRangeList(this);

	m_Mutex.Unlock();

	if (GetObjectManager())
	{
		GetObjectManager()->RemovePlayerFromMOBRangeLists(this);
	}
}

void Player::TowToBase()
{
    if (!m_SectorMgr) return;
    //for now, just transfer to registered starbase.
    long sector_id = m_RegisteredSectorID;//m_SectorMgr->GetSectorIDFromName(PlayerIndex()->GetRegistrationStarbaseSector());

	if (sector_id == 0)
    {
        //class starting sector, so there's always somewhere to go
        sector_id = StartSector[m_Database.avatar.race * 3 + m_Database.avatar.profession];
    }

    if (sector_id > 0)
    {
        long destination = sector_id;
        if (sector_id < 9999) destination = sector_id * 10 + 1;
        SetLoginCamera(0, 0);
        m_SectorMgr->SectorServerHandoff(this, destination);
    }
    else
    {
        SendVaMessage("Invalid Registered Starbase: %d", m_RegisteredSectorID/*PlayerIndex()->GetRegistrationStarbase()*/);
    }
}

bool Player::IsInSameSector(Player *p)
{
    if (m_PlayerIndex.GetSectorNum() == p->m_PlayerIndex.GetSectorNum())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Player::ShipUpgrade(long upgrade)
{
    //first check if hull upgrade is valid
    if ((upgrade - 1) != PlayerIndex()->RPGInfo.GetHullUpgradeLevel())
    {
        SendVaMessage("BUG: Invalid upgrade sent #%d current upgrade [%d]. Report to devs.", upgrade, PlayerIndex()->RPGInfo.GetHullUpgradeLevel());
        LogMessage("BUG: Invalid upgrade sent #%d current upgrade [%d].\n", upgrade, PlayerIndex()->RPGInfo.GetHullUpgradeLevel());
        return;
    }

    //verify hull upgrade is valid
    switch (upgrade)
    {
    case 1: //10
        if (TotalLevel() < 10)
        {
            return;
        }
        break;

    case 2: //30
        if (TotalLevel() < 30)
        {
            return;
        }
		ShipIndex()->BaseStats.SetWarpPowerLevel(4);
		ShipIndex()->CurrentStats.SetWarpPowerLevel(4);
        break;

    case 3: //50
        if (TotalLevel() >= 50)
        {
            //Take the assets from static data for consitency
	        m_Database.ship_info.hull         = 1 + BaseHullAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.hull];
	        m_Database.ship_info.profession   = 1 + BaseProfAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.profession];
	        m_Database.ship_info.wing         = 1 + BaseWingAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.wing];
	        m_Database.ship_info.engine       = 1 + BaseEngineAsset[m_Database.ship_data.race];
        }
        else
        {
            return;
        }
        break;

    case 4: //75
        if (TotalLevel() < 75)
        {
            return;
        }
		ShipIndex()->BaseStats.SetWarpPowerLevel(5);
		ShipIndex()->CurrentStats.SetWarpPowerLevel(5);
        break;

    case 5: //100
        if (TotalLevel() >= 100)
        {
            //Take the assets from static data for consitency
	        m_Database.ship_info.hull         = 2 + BaseHullAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.hull];
	        m_Database.ship_info.profession   = 2 + BaseProfAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.profession];
	        m_Database.ship_info.wing         = 2 + BaseWingAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.wing];
	        m_Database.ship_info.engine       = 2 + BaseEngineAsset[m_Database.ship_data.race];
        }
        else
        {
            return;
        }
        break;

    case 6: //135
        if (TotalLevel() < 135)
        {
            return;
        }
		ShipIndex()->BaseStats.SetWarpPowerLevel(6);
		ShipIndex()->CurrentStats.SetWarpPowerLevel(6);
        break;

    default:
        SendVaMessage("BUG: Invalid upgrade sent #%d. Report to devs.", upgrade);
        return;
        break;
	}

    ShipIndex()->SetHullPoints(float(HullTable[ClassIndex() * 7 + upgrade]));
    ShipIndex()->SetMaxHullPoints(float(HullTable[ClassIndex() * 7 + upgrade]));

    //do we add a weapon?
    if (WeaponTable[ClassIndex() * 7 + upgrade] != 0)
    {
        AddWeapon(m_WeaponSlots + 1);
    }

    //always add 2 to cargo slots
    long slots = ShipIndex()->Inventory.GetCargoSpace();

    if (slots < 40)
    {
        ShipIndex()->Inventory.SetCargoSpace(slots+2);
        ShipIndex()->Inventory.CargoInv.Item[slots].SetItemTemplateID(-1);
        ShipIndex()->Inventory.CargoInv.Item[slots+1].SetItemTemplateID(-1);
    }

    //add device slot if required
    if (DeviceTable[ClassIndex() * 7 + upgrade] != 0)
    {
        //see how many devices we have. Perhaps we could track this?
        short devices = 0;
        for (int i = 0; i < upgrade; i++)
        {
            devices += DeviceTable[ClassIndex()*7 + i];
        }

        ShipIndex()->Inventory.Mounts.SetMount(9+devices, DeviceMount);
		ShipIndex()->Inventory.EquipInv.EquipItem[9+devices].SetItemTemplateID(-1);
		m_DeviceSlots++;
    }

    PlayerIndex()->RPGInfo.SetHullUpgradeLevel(upgrade);

	SaveHullUpgrade();
	SaveHullLevelChange(ShipIndex()->GetMaxHullPoints());

    SendAuxShip();
    SendAuxPlayer();
    SaveDatabase();
}

void Player::ResetRangeLists()
{
    //memset(m_ObjRangeList, 0, MAX_OBJS_PER_SECTOR/8);
    memset(m_ResourceSendList, 0, MAX_OBJS_PER_SECTOR/8);
}

void Player::FinishLogin()
{       
    long entry_point_id = 0x6C;
    long camera_type = 0;
    Object *came_from = 0;

	ObjectManager *om = GetObjectManager();
    
    if (FromSector() > 9999 && om)
    {
        came_from = om->FindStation(0);
        if (came_from)
        {        
            entry_point_id = ntohl(came_from->GameID());
        }
        else
        {
            LogMessage("Sector Login - Failed to find entry point!\n");
            entry_point_id = 0;
        }
    }
    else if (FromSector() > 900)
    {
        camera_type = 0x02000000;
        entry_point_id = 0x00;
        came_from = om->FindGate(FromSector());
        if (came_from) came_from->BlipGate(GameID());
    }
    
    //player->SendCameraControl(camera_type, entry_point_id);
    SetLoginCamera(camera_type, entry_point_id);
    CheckNavs();
	
	SetInSpace(true);

	//player->m_Effects.SendEffects(player);

    //LogMessage("SECTOR login finish\n");

    // TODO: send ClientChatEvent to enable the sector channel
}
