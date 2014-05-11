#include "AuxShipIndex.h"

Mutex * AuxShipIndex::Buffer()
{
    return &m_BufferMutex;
}

bool AuxShipIndex::HasDiff()
{
    return ((Flags[0] & 0x02) != 0);
}

void AuxShipIndex::BuildDiff()
{
    /*Try to build the player diff packet, if it doesnt build,
      the others wont either so just return false

      NOTE: The buffer mutex must be locked before calling this function
            by the callee to ensure that no other packet can be built

            This assumes that the above method was called and a diff
            packet exists to be build, otherwise, you are wasting cpu cycles
    */

    /* Start building the create diff packet
       m_CreateDiff points to the first byte of m_Buffer */
    BuildCreatePacket(m_CreateDiff, m_CreateDiffLength);

    /* Same thing, build the click at the end of create */
    m_ClickDiff = &m_CreateDiff[m_CreateDiffLength];
    BuildClickPacket(m_ClickDiff, m_ClickDiffLength);

    /* The return value of BuildPacket is the return value of this method */
    m_Diff = &m_ClickDiff[m_ClickDiffLength];
    BuildPacket(m_Diff, m_DiffLength);
}

bool AuxShipIndex::BuildPacket(unsigned char *buffer, long &index)
{
	if (!GameID)
	{
		return false;
	}

    if ((Flags[0] & 0x02) == 0)
    {
        return false;
    }

    m_Mutex->Lock();

	index = 0;
	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[7] & 0x40
	{
		AddString(buffer,Data.Name,index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[7] & 0x80
	{
		AddString(buffer,Data.Owner,index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[8] & 0x01
	{
		AddString(buffer,Data.Title,index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[8] & 0x02
	{
		AddString(buffer,Data.Rank,index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[8] & 0x04
	{
	    Energy.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[8] & 0x08
	{
		AddData(buffer,Data.MaxEnergy,index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[8] & 0x10
	{
	    Shield.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[8] & 0x20
	{
		AddData(buffer,Data.MaxShield,index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[8] & 0x40
	{
		AddData(buffer,Data.HullPoints,index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[8] & 0x80
	{
		AddData(buffer,Data.MaxHullPoints,index);
	}

	if (Flags[1] & 0x40)	//ExtendedFlags[9] & 0x01
	{
		AddData(buffer,Data.MaxTiltRate,index);
	}

	if (Flags[1] & 0x80)	//ExtendedFlags[9] & 0x02
	{
		AddData(buffer,Data.MaxTurnRate,index);
	}

	if (Flags[2] & 0x01)	//ExtendedFlags[9] & 0x04
	{
		AddData(buffer,Data.MaxTiltAngle,index);
	}

	if (Flags[2] & 0x02)	//ExtendedFlags[9] & 0x08
	{
		AddData(buffer,Data.MaxSpeed,index);
	}

	if (Flags[2] & 0x04)	//ExtendedFlags[9] & 0x10
	{
		AddData(buffer,Data.MinSpeed,index);
	}

	if (Flags[2] & 0x08)	//ExtendedFlags[9] & 0x20
	{
		AddData(buffer,Data.Acceleration,index);
	}

	if (Flags[2] & 0x10)	//ExtendedFlags[9] & 0x40
	{
		AddData(buffer,char(Data.LockSpeed),index);
	}

	if (Flags[2] & 0x20)	//ExtendedFlags[9] & 0x80
	{
		AddData(buffer,char(Data.LockOrient),index);
	}

	if (Flags[2] & 0x40)	//ExtendedFlags[10] & 0x01
	{
		AddData(buffer,char(Data.AutoLevel),index);
	}

	if (Flags[2] & 0x80)	//ExtendedFlags[10] & 0x02
	{
		AddData(buffer,char(Data.IsCloaked),index);
	}

	if (Flags[3] & 0x01)	//ExtendedFlags[10] & 0x04
	{
		AddData(buffer,char(Data.IsCountermeasureActive),index);
	}

	if (Flags[3] & 0x02)	//ExtendedFlags[10] & 0x08
	{
		AddData(buffer,char(Data.IsIncapacitated),index);
	}

	if (Flags[3] & 0x04)	//ExtendedFlags[10] & 0x10
	{
		AddData(buffer,char(Data.IsOrganic),index);
	}

	if (Flags[3] & 0x08)	//ExtendedFlags[10] & 0x20
	{
		AddData(buffer,char(Data.IsInPVP),index);
	}

	if (Flags[3] & 0x10)	//ExtendedFlags[10] & 0x40
	{
		AddData(buffer,char(Data.IsAutoFollowing),index);
	}

	if (Flags[3] & 0x20)	//ExtendedFlags[10] & 0x80
	{
		AddData(buffer,char(Data.IsRescueBeaconActive),index);
	}

	if (Flags[3] & 0x40)	//ExtendedFlags[11] & 0x01
	{
		AddData(buffer,Data.CombatLevel,index);
	}

	if (Flags[3] & 0x80)	//ExtendedFlags[11] & 0x02
	{
		AddData(buffer,Data.TargetGameID,index);
	}

	if (Flags[4] & 0x01)	//ExtendedFlags[11] & 0x04
	{
		AddString(buffer,Data.TargetThreat,index);
	}

	if (Flags[4] & 0x02)	//ExtendedFlags[11] & 0x08
	{
		AddString(buffer,Data.TargetThreatSound,index);
	}

	if (Flags[4] & 0x04)	//ExtendedFlags[11] & 0x10
	{
		AddData(buffer,Data.TargetThreatLevel,index);
	}

	if (Flags[4] & 0x08)	//ExtendedFlags[11] & 0x20
	{
		AddData(buffer,Data.PrivateWarpState,index);
	}

	if (Flags[4] & 0x10)	//ExtendedFlags[11] & 0x40
	{
		AddData(buffer,Data.GlobalWarpState,index);
	}

	if (Flags[4] & 0x20)	//ExtendedFlags[11] & 0x80
	{
		AddData(buffer,Data.WarpAvailable,index);
	}

	if (Flags[4] & 0x40)	//ExtendedFlags[12] & 0x01
	{
		AddData(buffer,Data.WarpTriggerTime,index);
	}

	if (Flags[4] & 0x80)	//ExtendedFlags[12] & 0x02
	{
	    Shake.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x01)	//ExtendedFlags[12] & 0x04
	{
	    Inventory.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x02)	//ExtendedFlags[12] & 0x08
	{
	    QuadrantDamage.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x04)	//ExtendedFlags[12] & 0x10
	{
	    DamageSpot.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x08)	//ExtendedFlags[12] & 0x20
	{
	    DamageLine.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x10)	//ExtendedFlags[12] & 0x40
	{
	    DamageBlotch.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x20)	//ExtendedFlags[12] & 0x80
	{
	    Lego.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x40)	//ExtendedFlags[13] & 0x01
	{
	    Buffs.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x80)	//ExtendedFlags[13] & 0x02
	{
		AddData(buffer,Data.TradeMoney,index);
	}

	if (Flags[6] & 0x01)	//ExtendedFlags[13] & 0x04
	{
	    BaseStats.BuildPacket(buffer, index);
	}

	if (Flags[6] & 0x02)	//ExtendedFlags[13] & 0x08
	{
	    CurrentStats.BuildPacket(buffer, index);
	}

	if (Flags[6] & 0x04)	//ExtendedFlags[13] & 0x10
	{
		AddData(buffer,Data.EngineThrustState,index);
	}

	if (Flags[6] & 0x08)	//ExtendedFlags[13] & 0x20
	{
		AddData(buffer,Data.EngineTrailType,index);
	}

	if (Flags[6] & 0x10)	//ExtendedFlags[13] & 0x40
	{
		AddString(buffer,Data.GuildName,index);
	}

	if (Flags[6] & 0x20)	//ExtendedFlags[13] & 0x80
	{
		AddData(buffer,Data.GuildRank,index);
	}

	if (Flags[6] & 0x40)	//ExtendedFlags[14] & 0x01
	{
		AddString(buffer,Data.GuildRankName,index);
	}

	if (Flags[6] & 0x80)	//ExtendedFlags[14] & 0x02
	{
		AddData(buffer,Data.SameGuildTagColor[0],index);
		AddData(buffer,Data.SameGuildTagColor[1],index);
		AddData(buffer,Data.SameGuildTagColor[2],index);
	}

	if (Flags[7] & 0x01)	//ExtendedFlags[14] & 0x04
	{
		AddData(buffer,Data.OtherGuildTagColor[0],index);
		AddData(buffer,Data.OtherGuildTagColor[1],index);
		AddData(buffer,Data.OtherGuildTagColor[2],index);
	}

	if (Flags[7] & 0x02)	//ExtendedFlags[14] & 0x08
	{
		AddString(buffer,Data.InterruptAbilityName,index);
	}

	if (Flags[7] & 0x04)	//ExtendedFlags[14] & 0x10
	{
		AddData(buffer,Data.InterruptState,index);
	}

	if (Flags[7] & 0x08)	//ExtendedFlags[14] & 0x20
	{
		AddData(buffer,Data.InterruptActivationTime,index);
	}

	if (Flags[7] & 0x10)	//ExtendedFlags[14] & 0x40
	{
		AddData(buffer,Data.InterruptProgress,index);
	}

	if (Flags[7] & 0x20)	//ExtendedFlags[14] & 0x80
	{
		AddString(buffer,Data.FactionIdentifier,index);
	}

	*((short *) &buffer[4]) = short(index - 6);

	memset(Flags,0,sizeof(Flags));

    m_Mutex->Unlock();

	return true;
}

bool AuxShipIndex::BuildExtendedPacket()
{
	return BuildExtendedPacket(m_Packet, m_PacketLength);
}

bool AuxShipIndex::BuildExtendedPacket(unsigned char *buffer, long &index)
{
    /*
    NOTE: The buffer mutex must be locked before calling this function
          by the callee to ensure that no other packet can be built
    */

	if (!GameID)
	{
		return false;
	}

    if ((ExtendedFlags[0] & 0x02) == 0)
    {
        return false;
    }

    m_Mutex->Lock();

	index = 0;
	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[7] & 0x40
	{
		AddString(buffer,Data.Name,index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[7] & 0x80
	{
		AddString(buffer,Data.Owner,index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[8] & 0x01
	{
		AddString(buffer,Data.Title,index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[8] & 0x02
	{
		AddString(buffer,Data.Rank,index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[8] & 0x04
	{
	    Energy.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[8] & 0x04)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[8] & 0x08
	{
		AddData(buffer,Data.MaxEnergy,index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[8] & 0x10
	{
	    Shield.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[8] & 0x10)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[8] & 0x20
	{
		AddData(buffer,Data.MaxShield,index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[8] & 0x40
	{
		AddData(buffer,Data.HullPoints,index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[8] & 0x80
	{
		AddData(buffer,Data.MaxHullPoints,index);
	}

	if (ExtendedFlags[1] & 0x40)	//ExtendedFlags[9] & 0x01
	{
		AddData(buffer,Data.MaxTiltRate,index);
	}

	if (ExtendedFlags[1] & 0x80)	//ExtendedFlags[9] & 0x02
	{
		AddData(buffer,Data.MaxTurnRate,index);
	}

	if (ExtendedFlags[2] & 0x01)	//ExtendedFlags[9] & 0x04
	{
		AddData(buffer,Data.MaxTiltAngle,index);
	}

	if (ExtendedFlags[2] & 0x02)	//ExtendedFlags[9] & 0x08
	{
		AddData(buffer,Data.MaxSpeed,index);
	}

	if (ExtendedFlags[2] & 0x04)	//ExtendedFlags[9] & 0x10
	{
		AddData(buffer,Data.MinSpeed,index);
	}

	if (ExtendedFlags[2] & 0x08)	//ExtendedFlags[9] & 0x20
	{
		AddData(buffer,Data.Acceleration,index);
	}

	if (ExtendedFlags[2] & 0x10)	//ExtendedFlags[9] & 0x40
	{
		AddData(buffer,char(Data.LockSpeed),index);
	}

	if (ExtendedFlags[2] & 0x20)	//ExtendedFlags[9] & 0x80
	{
		AddData(buffer,char(Data.LockOrient),index);
	}

	if (ExtendedFlags[2] & 0x40)	//ExtendedFlags[10] & 0x01
	{
		AddData(buffer,char(Data.AutoLevel),index);
	}

	if (ExtendedFlags[2] & 0x80)	//ExtendedFlags[10] & 0x02
	{
		AddData(buffer,char(Data.IsCloaked),index);
	}

	if (ExtendedFlags[3] & 0x01)	//ExtendedFlags[10] & 0x04
	{
		AddData(buffer,char(Data.IsCountermeasureActive),index);
	}

	if (ExtendedFlags[3] & 0x02)	//ExtendedFlags[10] & 0x08
	{
		AddData(buffer,char(Data.IsIncapacitated),index);
	}

	if (ExtendedFlags[3] & 0x04)	//ExtendedFlags[10] & 0x10
	{
		AddData(buffer,char(Data.IsOrganic),index);
	}

	if (ExtendedFlags[3] & 0x08)	//ExtendedFlags[10] & 0x20
	{
		AddData(buffer,char(Data.IsInPVP),index);
	}

	if (ExtendedFlags[3] & 0x10)	//ExtendedFlags[10] & 0x40
	{
		AddData(buffer,char(Data.IsAutoFollowing),index);
	}

	if (ExtendedFlags[3] & 0x20)	//ExtendedFlags[10] & 0x80
	{
		AddData(buffer,char(Data.IsRescueBeaconActive),index);
	}

	if (ExtendedFlags[3] & 0x40)	//ExtendedFlags[11] & 0x01
	{
		AddData(buffer,Data.CombatLevel,index);
	}

	if (ExtendedFlags[3] & 0x80)	//ExtendedFlags[11] & 0x02
	{
		AddData(buffer,Data.TargetGameID,index);
	}

	if (ExtendedFlags[4] & 0x01)	//ExtendedFlags[11] & 0x04
	{
		AddString(buffer,Data.TargetThreat,index);
	}

	if (ExtendedFlags[4] & 0x02)	//ExtendedFlags[11] & 0x08
	{
		AddString(buffer,Data.TargetThreatSound,index);
	}

	if (ExtendedFlags[4] & 0x04)	//ExtendedFlags[11] & 0x10
	{
		AddData(buffer,Data.TargetThreatLevel,index);
	}

	if (ExtendedFlags[4] & 0x08)	//ExtendedFlags[11] & 0x20
	{
		AddData(buffer,Data.PrivateWarpState,index);
	}

	if (ExtendedFlags[4] & 0x10)	//ExtendedFlags[11] & 0x40
	{
		AddData(buffer,Data.GlobalWarpState,index);
	}

	if (ExtendedFlags[4] & 0x20)	//ExtendedFlags[11] & 0x80
	{
		AddData(buffer,Data.WarpAvailable,index);
	}

	if (ExtendedFlags[4] & 0x40)	//ExtendedFlags[12] & 0x01
	{
		AddData(buffer,Data.WarpTriggerTime,index);
	}

	if (ExtendedFlags[4] & 0x80)	//ExtendedFlags[12] & 0x02
	{
	    Shake.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x02)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x01)	//ExtendedFlags[12] & 0x04
	{
	    Inventory.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x04)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x02)	//ExtendedFlags[12] & 0x08
	{
	    QuadrantDamage.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x08)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x04)	//ExtendedFlags[12] & 0x10
	{
	    DamageSpot.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x10)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x08)	//ExtendedFlags[12] & 0x20
	{
	    DamageLine.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x20)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x10)	//ExtendedFlags[12] & 0x40
	{
	    DamageBlotch.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x40)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x20)	//ExtendedFlags[12] & 0x80
	{
	    Lego.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x80)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x40)	//ExtendedFlags[13] & 0x01
	{
	    Buffs.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[13] & 0x01)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x80)	//ExtendedFlags[13] & 0x02
	{
		AddData(buffer,Data.TradeMoney,index);
	}

	if (ExtendedFlags[6] & 0x01)	//ExtendedFlags[13] & 0x04
	{
	    BaseStats.BuildExtendedPacket(buffer, index);
    }
	else if (ExtendedFlags[13] & 0x04)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[6] & 0x02)	//ExtendedFlags[13] & 0x08
	{
	    CurrentStats.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[13] & 0x08)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[6] & 0x04)	//ExtendedFlags[13] & 0x10
	{
		AddData(buffer,Data.EngineThrustState,index);
	}

	if (ExtendedFlags[6] & 0x08)	//ExtendedFlags[13] & 0x20
	{
		AddData(buffer,Data.EngineTrailType,index);
	}

	if (ExtendedFlags[6] & 0x10)	//ExtendedFlags[13] & 0x40
	{
		AddString(buffer,Data.GuildName,index);
	}

	if (ExtendedFlags[6] & 0x20)	//ExtendedFlags[13] & 0x80
	{
		AddData(buffer,Data.GuildRank,index);
	}

	if (ExtendedFlags[6] & 0x40)	//ExtendedFlags[14] & 0x01
	{
		AddString(buffer,Data.GuildRankName,index);
	}

	if (ExtendedFlags[6] & 0x80)	//ExtendedFlags[14] & 0x02
	{
		AddData(buffer,Data.SameGuildTagColor[0],index);
		AddData(buffer,Data.SameGuildTagColor[1],index);
		AddData(buffer,Data.SameGuildTagColor[2],index);
	}

	if (ExtendedFlags[7] & 0x01)	//ExtendedFlags[14] & 0x04
	{
		AddData(buffer,Data.OtherGuildTagColor[0],index);
		AddData(buffer,Data.OtherGuildTagColor[1],index);
		AddData(buffer,Data.OtherGuildTagColor[2],index);
	}

	if (ExtendedFlags[7] & 0x02)	//ExtendedFlags[14] & 0x08
	{
		AddString(buffer,Data.InterruptAbilityName,index);
	}

	if (ExtendedFlags[7] & 0x04)	//ExtendedFlags[14] & 0x10
	{
		AddData(buffer,Data.InterruptState,index);
	}

	if (ExtendedFlags[7] & 0x08)	//ExtendedFlags[14] & 0x20
	{
		AddData(buffer,Data.InterruptActivationTime,index);
	}

	if (ExtendedFlags[7] & 0x10)	//ExtendedFlags[14] & 0x40
	{
		AddData(buffer,Data.InterruptProgress,index);
	}

	if (ExtendedFlags[7] & 0x20)	//ExtendedFlags[14] & 0x80
	{
		AddString(buffer,Data.FactionIdentifier,index);
	}

	*((short *) &buffer[4]) = short(index - 6);

    m_Mutex->Unlock();

    return true;
}

bool AuxShipIndex::BuildCreatePacket(unsigned char *buffer, long &index)
{
    if (!GameID)
	{
		return false;
	}

    if ((Flags[0] & 0x02) == 0)
    {
        return false;
    }

    m_Mutex->Lock();

	index = 0;
	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	/*
	If any of these change, this packet will contain it

	0, 1, 2, 3, 6, 8, 9, 19, 20, 21, 22, 23, 25, 26,
	32, 37, 38, 39, 40, 41, 46, 47, 48, 49, 50, 51, 52

    Name(0), Owner(1), Title(2), Rank(3), Shield (6), Hull(8), MaxHull(9), IsCloaked(19),
    IsCountermeasureActive(20), IsIncapacitated(21), IsOrganic(22), IsInPVP(23),
    IsRescueBeaconActive(25), GlobalWarpState(32), QuadrantDamage(37),DamageSpot(38),
    DamageLine(39), DamageBlotch(40), Legos(41), EngineThrustState(46), EngineTrailType(47),
	GuildName(48), GuildRank(49), GuildRankName(50), SameGuildTagColor(51), OtherGuildTagColor(52)
    */

	AddData(buffer, Flags[0], index);				// Indexes 0,1,2,3 and lower nibble
	AddData(buffer, char(Flags[1] & 0x34), index);	// Indexes 6,8,9
	AddData(buffer, char(Flags[2] & 0x80), index);	// Indexes 19
	AddData(buffer, char(Flags[3] & 0x3F), index);	// Indexes 20,21,22,23,24,25
	AddData(buffer, char(Flags[4] & 0x10), index);	// Indexes 32
	AddData(buffer, char(Flags[5] & 0x3E), index);	// Indexes 37,38,39,40,41
	AddData(buffer, char(Flags[6] & 0xFC), index);	// Indexes 46,47,48,49,50,51
	AddData(buffer, char(Flags[7] & 0x01), index);	// Indexes 52

	if (Flags[0] & 0x10)	//ExtendedFlags[7] & 0x40
	{
		AddString(buffer,Data.Name,index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[7] & 0x80
	{
		AddString(buffer,Data.Owner,index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[8] & 0x01
	{
		AddString(buffer,Data.Title,index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[8] & 0x02
	{
		AddString(buffer,Data.Rank,index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[8] & 0x10
	{
		Shield.BuildSpecialPacket(buffer, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[8] & 0x40
	{
		AddData(buffer,Data.HullPoints,index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[8] & 0x80
	{
		AddData(buffer,Data.MaxHullPoints,index);
	}

	if (Flags[2] & 0x80)	//ExtendedFlags[10] & 0x02
	{
		AddData(buffer,char(Data.IsCloaked),index);
	}

	if (Flags[3] & 0x01)	//ExtendedFlags[10] & 0x04
	{
		AddData(buffer,char(Data.IsCountermeasureActive),index);
	}

	if (Flags[3] & 0x02)	//ExtendedFlags[10] & 0x08
	{
		AddData(buffer,char(Data.IsIncapacitated),index);
	}

	if (Flags[3] & 0x04)	//ExtendedFlags[10] & 0x10
	{
		AddData(buffer,char(Data.IsOrganic),index);
	}

	if (Flags[3] & 0x08)	//ExtendedFlags[10] & 0x20
	{
		AddData(buffer,char(Data.IsInPVP),index);
	}

	if (Flags[3] & 0x10)	//ExtendedFlags[10] & 0x40
	{
		AddData(buffer,char(Data.IsAutoFollowing),index);
	}

	if (Flags[3] & 0x20)	//ExtendedFlags[10] & 0x80
	{
		AddData(buffer,char(Data.IsRescueBeaconActive),index);
	}

	if (Flags[4] & 0x10)	//ExtendedFlags[11] & 0x40
	{
		AddData(buffer,Data.GlobalWarpState,index);
	}

	if (Flags[5] & 0x02)	//ExtendedFlags[12] & 0x08
	{
	    QuadrantDamage.BuildSpecialPacket(buffer, index);
	}

	if (Flags[5] & 0x04)	//ExtendedFlags[12] & 0x10
	{
	    DamageSpot.BuildSpecialPacket(buffer, index);
	}

	if (Flags[5] & 0x08)	//ExtendedFlags[12] & 0x20
	{
	    DamageLine.BuildSpecialPacket(buffer, index);
	}

	if (Flags[5] & 0x10)	//ExtendedFlags[12] & 0x40
	{
	    DamageBlotch.BuildSpecialPacket(buffer, index);
	}

	if (Flags[5] & 0x20)	//ExtendedFlags[12] & 0x80
	{
	    Lego.BuildSpecialPacket(buffer, index);
	}

	if (Flags[6] & 0x04)	//ExtendedFlags[13] & 0x10
	{
		AddData(buffer,Data.EngineThrustState,index);
	}

	if (Flags[6] & 0x08)	//ExtendedFlags[13] & 0x20
	{
		AddData(buffer,Data.EngineTrailType,index);
	}

	if (Flags[6] & 0x10)	//ExtendedFlags[13] & 0x40
	{
		AddString(buffer,Data.GuildName,index);
	}

	if (Flags[6] & 0x20)	//ExtendedFlags[13] & 0x80
	{
		AddData(buffer,Data.GuildRank,index);
	}

	if (Flags[6] & 0x40)	//ExtendedFlags[14] & 0x01
	{
		AddString(buffer,Data.GuildRankName,index);
	}

	if (Flags[6] & 0x80)	//ExtendedFlags[14] & 0x02
	{
		AddData(buffer,Data.SameGuildTagColor[0],index);
		AddData(buffer,Data.SameGuildTagColor[1],index);
		AddData(buffer,Data.SameGuildTagColor[2],index);
	}

	if (Flags[7] & 0x01)	//ExtendedFlags[14] & 0x04
	{
		AddData(buffer,Data.OtherGuildTagColor[0],index);
		AddData(buffer,Data.OtherGuildTagColor[1],index);
		AddData(buffer,Data.OtherGuildTagColor[2],index);
	}

	*((short *) &buffer[4]) = short(index - 6);

    m_Mutex->Unlock();

    //If none of the flags were set, index is 4 + 2 + 1 + 8 = 15
    if (index == 15)
    {
        index = 0;
        return false;
    }

    return true;
}

bool AuxShipIndex::BuildCreateExtendedPacket()
{
	return BuildCreateExtendedPacket(m_Packet, m_PacketLength);
}

bool AuxShipIndex::BuildCreateExtendedPacket(unsigned char *buffer, long &index)
{
    /*
    NOTE: The buffer mutex must be locked before calling this function
          by the callee to ensure that no other packet can be built
    */

    if (!GameID)
	{
		return false;
	}

    m_Mutex->Lock();

	index = 0;
	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	/*
	This player is being created for someone else and has not been targeted so therefore
    cannot see the shields, combat level, faction, or interrupt

	0, 1, 2, 3, 8, 9, 19, 20, 21, 22, 23, 25, 26,
	32, 37, 38, 39, 40, 41, 46, 47, 48, 49, 50, 51, 52

    Name(0), Owner(1), Title(2), Rank(3), Hull(8), MaxHull(9), IsCloaked(19),
    IsCountermeasureActive(20), IsIncapacitated(21), IsOrganic(22), IsInPVP(23),
    IsRescueBeaconActive(25), GlobalWarpState(32), QuadrantDamage(37),DamageSpot(38),
    DamageLine(39), DamageBlotch(40), Legos(41), EngineThrustState(46), EngineTrailType(47),
	GuildName(48), GuildRank(49), GuildRankName(50), SameGuildTagColor(51), OtherGuildTagColor(52)
    */

	AddData(buffer, ExtendedFlags[0], index);				// Indexes 0,1,2,3 and lower nibble
	AddData(buffer, char(ExtendedFlags[1] & 0x30), index);	// Indexes 8,9
	AddData(buffer, char(ExtendedFlags[2] & 0x80), index);	// Indexes 19
	AddData(buffer, char(ExtendedFlags[3] & 0x3F), index);	// Indexes 20,21,22,23,24,25
	AddData(buffer, char(ExtendedFlags[4] & 0x10), index);	// Indexes 32
	AddData(buffer, char(ExtendedFlags[5] & 0x3E), index);	// Indexes 37,38,39,40,41
	AddData(buffer, char(ExtendedFlags[6] & 0xFC), index);	// Indexes 46,47,48,49,50,51
	AddData(buffer, char(ExtendedFlags[7] & 0xC1), index);	// Indexes 52 - 0,1
	AddData(buffer, char(ExtendedFlags[8] & 0xC3), index);	// Indexes 2,3,8,9
	AddData(buffer, char(0), index);						// None
	AddData(buffer, char(ExtendedFlags[10] & 0xFE), index);	// Indexes 19,20,21,22,23,24,25
	AddData(buffer, char(ExtendedFlags[11] & 0x40), index);	// Indexes 32
	AddData(buffer, char(ExtendedFlags[12] & 0xF8), index);	// Indexes 37,38,39,40,41
	AddData(buffer, char(ExtendedFlags[13] & 0xF0), index);	// Indexes 46,47,48,49
	AddData(buffer, char(ExtendedFlags[14] & 0x07), index);	// Indexes 50,51,52

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[7] & 0x40
	{
		AddString(buffer,Data.Name,index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[7] & 0x80
	{
		AddString(buffer,Data.Owner,index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[8] & 0x01
	{
		AddString(buffer,Data.Title,index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[8] & 0x02
	{
		AddString(buffer,Data.Rank,index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[8] & 0x40
	{
		AddData(buffer,Data.HullPoints,index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[8] & 0x80
	{
		AddData(buffer,Data.MaxHullPoints,index);
	}

	if (ExtendedFlags[2] & 0x80)	//ExtendedFlags[10] & 0x02
	{
		AddData(buffer,char(Data.IsCloaked),index);
	}

	if (ExtendedFlags[3] & 0x01)	//ExtendedFlags[10] & 0x04
	{
		AddData(buffer,char(Data.IsCountermeasureActive),index);
	}

	if (ExtendedFlags[3] & 0x02)	//ExtendedFlags[10] & 0x08
	{
		AddData(buffer,char(Data.IsIncapacitated),index);
	}

	if (ExtendedFlags[3] & 0x04)	//ExtendedFlags[10] & 0x10
	{
		AddData(buffer,char(Data.IsOrganic),index);
	}

	if (ExtendedFlags[3] & 0x08)	//ExtendedFlags[10] & 0x20
	{
		AddData(buffer,char(Data.IsInPVP),index);
	}

	if (ExtendedFlags[3] & 0x10)	//ExtendedFlags[10] & 0x40
	{
		AddData(buffer,char(Data.IsAutoFollowing),index);
	}

	if (ExtendedFlags[3] & 0x20)	//ExtendedFlags[10] & 0x80
	{
		AddData(buffer,char(Data.IsRescueBeaconActive),index);
	}

	if (ExtendedFlags[4] & 0x10)	//ExtendedFlags[11] & 0x40
	{
		AddData(buffer,Data.GlobalWarpState,index);
	}

	if (ExtendedFlags[5] & 0x02)	//ExtendedFlags[12] & 0x08
	{
	    QuadrantDamage.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x08)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x04)	//ExtendedFlags[12] & 0x10
	{
	    DamageSpot.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x10)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x08)	//ExtendedFlags[12] & 0x20
	{
	    DamageLine.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x20)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x10)	//ExtendedFlags[12] & 0x40
	{
	    DamageBlotch.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x40)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[5] & 0x20)	//ExtendedFlags[12] & 0x80
	{
	    Lego.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[12] & 0x80)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[6] & 0x04)	//ExtendedFlags[13] & 0x10
	{
		AddData(buffer,Data.EngineThrustState,index);
	}

	if (ExtendedFlags[6] & 0x08)	//ExtendedFlags[13] & 0x20
	{
		AddData(buffer,Data.EngineTrailType,index);
	}

	if (ExtendedFlags[6] & 0x10)	//ExtendedFlags[13] & 0x40
	{
		AddString(buffer,Data.GuildName,index);
	}

	if (ExtendedFlags[6] & 0x20)	//ExtendedFlags[13] & 0x80
	{
		AddData(buffer,Data.GuildRank,index);
	}

	if (ExtendedFlags[6] & 0x40)	//ExtendedFlags[14] & 0x01
	{
		AddString(buffer,Data.GuildRankName,index);
	}

	if (ExtendedFlags[6] & 0x80)	//ExtendedFlags[14] & 0x02
	{
		AddData(buffer,Data.SameGuildTagColor[0],index);
		AddData(buffer,Data.SameGuildTagColor[1],index);
		AddData(buffer,Data.SameGuildTagColor[2],index);
	}

	if (ExtendedFlags[7] & 0x01)	//ExtendedFlags[14] & 0x04
	{
		AddData(buffer,Data.OtherGuildTagColor[0],index);
		AddData(buffer,Data.OtherGuildTagColor[1],index);
		AddData(buffer,Data.OtherGuildTagColor[2],index);
	}

	*((short *) &buffer[4]) = short(index - 6);

    m_Mutex->Unlock();

	return true;
}

bool AuxShipIndex::BuildClickPacket(unsigned char *buffer, long &index)
{
	if (!GameID)
	{
		return false;
	}

    if ((Flags[0] & 0x02) == 0)
    {
        return false;
    }

    m_Mutex->Lock();

	index = 0;
	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	/*
    If any of the member sent in the click packet change, they are build here

	6, 7, 26, 53, 54, 55, 56, 57

    Shield(6), MaxShield(7), CombatLevel(26), InterruptAbilityName(53), InterruptState(54),
	InterruptActivationTime(55), InterruptProgress(56), FactionIdentifier(57)
    */

	AddData(buffer, char(0x02), index);				// 1-bit flag
	AddData(buffer, char(Flags[1] & 0x0C), index);	// Indexes 6,7
	AddData(buffer, char(0), index);				// None
	AddData(buffer, char(Flags[3] & 0x40), index);	// Indexes 26
	AddData(buffer, char(0), index);				// None
	AddData(buffer, char(0), index);				// None
	AddData(buffer, char(0), index);				// None
	AddData(buffer, char(Flags[7] & 0x3E), index);	// Indexes 53,54,55,56,57

	if (Flags[1] & 0x04)	//ExtendedFlags[8] & 0x10
	{
	    Shield.BuildSpecialPacket(buffer, index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[8] & 0x20
	{
		AddData(buffer,Data.MaxShield,index);
	}

	if (Flags[3] & 0x40)	//ExtendedFlags[11] & 0x01
	{
		AddData(buffer,Data.CombatLevel,index);
	}

	if (Flags[7] & 0x02)	//ExtendedFlags[14] & 0x08
	{
		AddString(buffer,Data.InterruptAbilityName,index);
	}

	if (Flags[7] & 0x04)	//ExtendedFlags[14] & 0x10
	{
		AddData(buffer,Data.InterruptState,index);
	}

	if (Flags[7] & 0x08)	//ExtendedFlags[14] & 0x20
	{
		AddData(buffer,Data.InterruptActivationTime,index);
	}

	if (Flags[7] & 0x10)	//ExtendedFlags[14] & 0x40
	{
		AddData(buffer,Data.InterruptProgress,index);
	}

	if (Flags[7] & 0x20)	//ExtendedFlags[14] & 0x80
	{
		AddString(buffer,Data.FactionIdentifier,index);
	}

	*((short *) &buffer[4]) = short(index - 6);

    m_Mutex->Unlock();

    //If none of the flags were set, index is 4 + 2 + 1 + 8 = 15
    if (index == 15)
    {
        index = 0;
        return false;
    }

    return true;
}

bool AuxShipIndex::BuildClickExtendedPacket()
{
	return BuildClickExtendedPacket(m_Packet, m_PacketLength);
}

bool AuxShipIndex::BuildClickExtendedPacket(unsigned char *buffer, long &index)
{
    /*
    NOTE: The buffer mutex must be locked before calling this function
          by the callee to ensure that no other packet can be built
    */

    if (!GameID)
	{
		return false;
	}

    m_Mutex->Lock();

	index = 0;
	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	/*
    By this point, the initial information should have been sent already
    so therefore this packet is relatively small

	6, 7, 26, 53, 54, 55, 56, 57

    Shield(6), MaxShield(7), CombatLevel(26), InterruptAbilityName(53), InterruptState(54),
	InterruptActivationTime(55), InterruptProgress(56), FactionIdentifier(57)
    */

	AddData(buffer, char(0x06), index);				        // 2-bit flag
	AddData(buffer, char(ExtendedFlags[1] & 0x0C), index);	// Indexes 6,7
	AddData(buffer, char(0), index);						// None
	AddData(buffer, char(ExtendedFlags[3] & 0x40), index);	// Indexes 26
	AddData(buffer, char(0), index);						// None
	AddData(buffer, char(0), index);						// None
	AddData(buffer, char(0), index);						// None
	AddData(buffer, char(ExtendedFlags[7] & 0x3E), index);	// Indexes 53,54,55,56,57
	AddData(buffer, char(ExtendedFlags[8] & 0x30), index);	// Indexes 6,7
	AddData(buffer, char(0), index);						// None
	AddData(buffer, char(0), index);						// None
	AddData(buffer, char(ExtendedFlags[11] & 0x01), index);	// Indexes 26
	AddData(buffer, char(0), index);						// None
	AddData(buffer, char(0), index);						// None
	AddData(buffer, char(ExtendedFlags[14] & 0xF8), index);	// Indexes 53,54,55,56,57

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[8] & 0x10
	{
	    Shield.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[8] & 0x10)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[8] & 0x20
	{
		AddData(buffer,Data.MaxShield,index);
	}

	if (ExtendedFlags[3] & 0x40)	//ExtendedFlags[11] & 0x01
	{
		AddData(buffer,Data.CombatLevel,index);
	}

	if (ExtendedFlags[7] & 0x02)	//ExtendedFlags[14] & 0x08
	{
		AddString(buffer,Data.InterruptAbilityName,index);
	}

	if (ExtendedFlags[7] & 0x04)	//ExtendedFlags[14] & 0x10
	{
		AddData(buffer,Data.InterruptState,index);
	}

	if (ExtendedFlags[7] & 0x08)	//ExtendedFlags[14] & 0x20
	{
		AddData(buffer,Data.InterruptActivationTime,index);
	}

	if (ExtendedFlags[7] & 0x10)	//ExtendedFlags[14] & 0x40
	{
		AddData(buffer,Data.InterruptProgress,index);
	}

	if (ExtendedFlags[7] & 0x20)	//ExtendedFlags[14] & 0x80
	{
		AddString(buffer,Data.FactionIdentifier,index);
	}

	*((short *) &buffer[4]) = short(index - 6);

    m_Mutex->Unlock();

	return true;
}

/******************************
*         GET METHODS         *
******************************/

_ShipIndex * AuxShipIndex::GetData()                {return &Data;}

u32 AuxShipIndex::GetGameID()						{return GameID;}

char * AuxShipIndex::GetName()						{return Data.Name;}
char * AuxShipIndex::GetOwner()						{return Data.Owner;}
char * AuxShipIndex::GetTitle()						{return Data.Title;}
char * AuxShipIndex::GetRank()						{return Data.Rank;}
float AuxShipIndex::GetMaxEnergy()					{return Data.MaxEnergy;}
float AuxShipIndex::GetMaxShield()					{return Data.MaxShield;}
float AuxShipIndex::GetHullPoints()					{return Data.HullPoints;}
float AuxShipIndex::GetMaxHullPoints()				{return Data.MaxHullPoints;}
float AuxShipIndex::GetMaxTiltRate()				{return Data.MaxTiltRate;}
float AuxShipIndex::GetMaxTurnRate()				{return Data.MaxTurnRate;}
float AuxShipIndex::GetMaxTiltAngle()				{return Data.MaxTiltAngle;}
float AuxShipIndex::GetMaxSpeed()					{return Data.MaxSpeed;}
float AuxShipIndex::GetMinSpeed()					{return Data.MinSpeed;}
float AuxShipIndex::GetAcceleration()				{return Data.Acceleration;}
bool AuxShipIndex::GetLockSpeed()					{return Data.LockSpeed;}
bool AuxShipIndex::GetLockOrient()					{return Data.LockOrient;}
bool AuxShipIndex::GetAutoLevel()					{return Data.AutoLevel;}
bool AuxShipIndex::GetIsCloaked()					{return Data.IsCloaked;}
bool AuxShipIndex::GetIsCountermeasureActive()		{return Data.IsCountermeasureActive;}
bool AuxShipIndex::GetIsIncapacitated()				{return Data.IsIncapacitated;}
bool AuxShipIndex::GetIsOrganic()					{return Data.IsOrganic;}
bool AuxShipIndex::GetIsInPVP()						{return Data.IsInPVP;}
bool AuxShipIndex::GetIsAutoFollowing()				{return Data.IsAutoFollowing;}
bool AuxShipIndex::GetIsRescueBeaconActive()		{return Data.IsRescueBeaconActive;}
u32 AuxShipIndex::GetCombatLevel()					{return Data.CombatLevel;}
u32 AuxShipIndex::GetTargetGameID()					{return Data.TargetGameID;}
char * AuxShipIndex::GetTargetThreat()				{return Data.TargetThreat;}
char * AuxShipIndex::GetTargetThreatSound()			{return Data.TargetThreatSound;}
u32 AuxShipIndex::GetTargetThreatLevel()			{return Data.TargetThreatLevel;}
u32 AuxShipIndex::GetPrivateWarpState()				{return Data.PrivateWarpState;}
u32 AuxShipIndex::GetGlobalWarpState()				{return Data.GlobalWarpState;}
u32 AuxShipIndex::GetWarpAvailable()				{return Data.WarpAvailable;}
u32 AuxShipIndex::GetWarpTriggerTime()				{return Data.WarpTriggerTime;}
u64 AuxShipIndex::GetTradeMoney()					{return Data.TradeMoney;}
u32 AuxShipIndex::GetEngineThrustState()			{return Data.EngineThrustState;}
u32 AuxShipIndex::GetEngineTrailType()				{return Data.EngineTrailType;}
char * AuxShipIndex::GetGuildName()					{return Data.GuildName;}
u32 AuxShipIndex::GetGuildRank()					{return Data.GuildRank;}
char * AuxShipIndex::GetGuildRankName()				{return Data.GuildRankName;}
float * AuxShipIndex::GetSameGuildTagColor()		{return Data.SameGuildTagColor;}
float * AuxShipIndex::GetOtherGuildTagColor()		{return Data.OtherGuildTagColor;}
char * AuxShipIndex::GetInterruptAbilityName()	    {return Data.InterruptAbilityName;}
u32 AuxShipIndex::GetInterruptState()				{return Data.InterruptState;}
u32 AuxShipIndex::GetInterruptActivationTime()	    {return Data.InterruptActivationTime;}
float AuxShipIndex::GetInterruptProgress()			{return Data.InterruptProgress;}
char * AuxShipIndex::GetFactionIdentifier()			{return Data.FactionIdentifier;}

/******************************
*         SET METHODS         *
******************************/

void AuxShipIndex::SetData(_ShipIndex *NewData)
{
	ReplaceString(Data.Name, NewData->Name, 0);
	ReplaceString(Data.Owner, NewData->Owner, 1);
	ReplaceString(Data.Title, NewData->Title, 2);
	ReplaceString(Data.Rank, NewData->Rank, 3);
	Energy.SetData(&NewData->Energy);
	ReplaceData(Data.MaxEnergy, NewData->MaxEnergy, 5);
	Shield.SetData(&NewData->Shield);
	ReplaceData(Data.MaxShield, NewData->MaxShield, 7);
	ReplaceData(Data.HullPoints, NewData->HullPoints, 8);
	ReplaceData(Data.MaxHullPoints, NewData->MaxHullPoints, 9);
	ReplaceData(Data.MaxTiltRate, NewData->MaxTiltRate, 10);
	ReplaceData(Data.MaxTurnRate, NewData->MaxTurnRate, 11);
	ReplaceData(Data.MaxTiltAngle, NewData->MaxTiltAngle, 12);
	ReplaceData(Data.MaxSpeed, NewData->MaxSpeed, 13);
	ReplaceData(Data.MinSpeed, NewData->MinSpeed, 14);
	ReplaceData(Data.Acceleration, NewData->Acceleration, 15);
	ReplaceData(Data.LockSpeed, NewData->LockSpeed, 16);
	ReplaceData(Data.LockOrient, NewData->LockOrient, 17);
	ReplaceData(Data.AutoLevel, NewData->AutoLevel, 18);
	ReplaceData(Data.IsCloaked, NewData->IsCloaked, 19);
	ReplaceData(Data.IsCountermeasureActive, NewData->IsCountermeasureActive, 20);
	ReplaceData(Data.IsIncapacitated, NewData->IsIncapacitated, 21);
	ReplaceData(Data.IsOrganic, NewData->IsOrganic, 22);
	ReplaceData(Data.IsInPVP, NewData->IsInPVP, 23);
	ReplaceData(Data.IsAutoFollowing, NewData->IsAutoFollowing, 24);
	ReplaceData(Data.IsRescueBeaconActive, NewData->IsRescueBeaconActive, 25);
	ReplaceData(Data.CombatLevel, NewData->CombatLevel, 26);
	ReplaceData(Data.TargetGameID, NewData->TargetGameID, 27);
	ReplaceString(Data.TargetThreat, NewData->TargetThreat, 28);
	ReplaceString(Data.TargetThreatSound, NewData->TargetThreatSound, 29);
	ReplaceData(Data.TargetThreatLevel, NewData->TargetThreatLevel, 30);
	ReplaceData(Data.PrivateWarpState, NewData->PrivateWarpState, 31);
	ReplaceData(Data.GlobalWarpState, NewData->GlobalWarpState, 32);
	ReplaceData(Data.WarpAvailable, NewData->WarpAvailable, 33);
	ReplaceData(Data.WarpTriggerTime, NewData->WarpTriggerTime, 34);
	Shake.SetData(&NewData->Shake);
	Inventory.SetData(&NewData->Inventory);
	QuadrantDamage.SetData(&NewData->QuadrantDamage);
	DamageSpot.SetData(&NewData->DamageSpot);
	DamageLine.SetData(&NewData->DamageLine);
	DamageBlotch.SetData(&NewData->DamageBlotch);
	Lego.SetData(&NewData->Lego);
	Buffs.SetData(&NewData->Buffs);
	ReplaceData(Data.TradeMoney, NewData->TradeMoney, 43);
	BaseStats.SetData(&NewData->BaseStats);
	CurrentStats.SetData(&NewData->CurrentStats);
	ReplaceData(Data.EngineThrustState, NewData->EngineThrustState, 46);
	ReplaceData(Data.EngineTrailType, NewData->EngineTrailType, 47);
	ReplaceString(Data.GuildName, NewData->GuildName, 48);
	ReplaceData(Data.GuildRank, NewData->GuildRank, 49);
	ReplaceString(Data.GuildRankName, NewData->GuildRankName, 50);
	ReplaceColor(Data.SameGuildTagColor, NewData->SameGuildTagColor, 51);
	ReplaceColor(Data.OtherGuildTagColor, NewData->OtherGuildTagColor, 52);
	ReplaceString(Data.InterruptAbilityName, NewData->InterruptAbilityName, 53);
	ReplaceData(Data.InterruptState, NewData->InterruptState, 54);
	ReplaceData(Data.InterruptActivationTime, NewData->InterruptActivationTime, 55);
	ReplaceData(Data.InterruptProgress, NewData->InterruptProgress, 56);
	ReplaceString(Data.FactionIdentifier, NewData->FactionIdentifier, 57);
}

void AuxShipIndex::SetGameID(u32 NewGameID)
{
	GameID = NewGameID;
}

void AuxShipIndex::SetName(const char *NewName)
{
	ReplaceString(Data.Name, NewName, 0);
}

void AuxShipIndex::SetOwner(const char *NewOwner)
{
	ReplaceString(Data.Owner, NewOwner, 1);
}

void AuxShipIndex::SetTitle(const char *NewTitle)
{
	ReplaceString(Data.Title, NewTitle, 2);
}

void AuxShipIndex::SetRank(const char *NewRank)
{
	ReplaceString(Data.Rank, NewRank, 3);
}

void AuxShipIndex::SetMaxEnergy(float NewMaxEnergy)
{
	ReplaceData(Data.MaxEnergy, NewMaxEnergy, 5);
}

void AuxShipIndex::SetMaxShield(float NewMaxShield)
{
	ReplaceData(Data.MaxShield, NewMaxShield, 7);
}

void AuxShipIndex::SetHullPoints(float NewHullPoints)
{
	ReplaceData(Data.HullPoints, NewHullPoints, 8);
}

void AuxShipIndex::SetMaxHullPoints(float NewMaxHullPoints)
{
	ReplaceData(Data.MaxHullPoints, NewMaxHullPoints, 9);
}

void AuxShipIndex::SetMaxTiltRate(float NewMaxTiltRate)
{
	ReplaceData(Data.MaxTiltRate, NewMaxTiltRate, 10);
}

void AuxShipIndex::SetMaxTurnRate(float NewMaxTurnRate)
{
	ReplaceData(Data.MaxTurnRate, NewMaxTurnRate, 11);
}

void AuxShipIndex::SetMaxTiltAngle(float NewMaxTiltAngle)
{
	ReplaceData(Data.MaxTiltAngle, NewMaxTiltAngle, 12);
}

void AuxShipIndex::SetMaxSpeed(float NewMaxSpeed)
{
	ReplaceData(Data.MaxSpeed, NewMaxSpeed, 13);
}

void AuxShipIndex::SetMinSpeed(float NewMinSpeed)
{
	ReplaceData(Data.MinSpeed, NewMinSpeed, 14);
}

void AuxShipIndex::SetAcceleration(float NewAcceleration)
{
	ReplaceData(Data.Acceleration, NewAcceleration, 15);
}

void AuxShipIndex::SetLockSpeed(bool NewLockSpeed)
{
	ReplaceData(Data.LockSpeed, NewLockSpeed, 16);
}

void AuxShipIndex::SetLockOrient(bool NewLockOrient)
{
	ReplaceData(Data.LockOrient, NewLockOrient, 17);
}

void AuxShipIndex::SetAutoLevel(bool NewAutoLevel)
{
	ReplaceData(Data.AutoLevel, NewAutoLevel, 18);
}

void AuxShipIndex::SetIsCloaked(bool NewIsCloaked)
{
	ReplaceData(Data.IsCloaked, NewIsCloaked, 19);
}

void AuxShipIndex::SetIsCountermeasureActive(bool NewIsCountermeasureActive)
{
	ReplaceData(Data.IsCountermeasureActive, NewIsCountermeasureActive, 20);
}

void AuxShipIndex::SetIsIncapacitated(bool NewIsIncapacitated)
{
	ReplaceData(Data.IsIncapacitated, NewIsIncapacitated, 21);
	LogMessage("Set incapacitated: %s\n", NewIsIncapacitated ? "true" : "false" );
}

void AuxShipIndex::SetIsOrganic(bool NewIsOrganic)
{
	ReplaceData(Data.IsOrganic, NewIsOrganic, 22);
}

void AuxShipIndex::SetIsInPVP(bool NewIsInPVP)
{
	ReplaceData(Data.IsInPVP, NewIsInPVP, 23);
}

void AuxShipIndex::SetIsAutoFollowing(bool NewIsAutoFollowing)
{
	ReplaceData(Data.AutoLevel, NewIsAutoFollowing, 24);
}

void AuxShipIndex::SetIsRescueBeaconActive(bool NewIsRescueBeaconActive)
{
	ReplaceData(Data.IsRescueBeaconActive, NewIsRescueBeaconActive, 25);
}

void AuxShipIndex::SetCombatLevel(u32 NewCombatLevel)
{
	ReplaceData(Data.CombatLevel, NewCombatLevel, 26);
}

void AuxShipIndex::SetTargetGameID(u32 NewTargetGameID)
{
	ReplaceData(Data.TargetGameID, NewTargetGameID, 27);
}

void AuxShipIndex::SetTargetThreat(const char *NewTargetThreat)
{
	ReplaceString(Data.TargetThreat, NewTargetThreat, 28);
}

void AuxShipIndex::SetTargetThreatSound(const char *NewTargetThreatSound)
{
	ReplaceString(Data.TargetThreatSound, NewTargetThreatSound, 29);
}

void AuxShipIndex::SetTargetThreatLevel(u32 NewTargetThreatLevel)
{
	ReplaceData(Data.TargetThreatLevel, NewTargetThreatLevel, 30);
}

void AuxShipIndex::SetPrivateWarpState(u32 NewPrivateWarpState)
{
	ReplaceData(Data.PrivateWarpState, NewPrivateWarpState, 31);
}

void AuxShipIndex::SetGlobalWarpState(u32 NewGlobalWarpState)
{
	ReplaceData(Data.GlobalWarpState, NewGlobalWarpState, 32);
}

void AuxShipIndex::SetWarpAvailable(u32 NewWarpAvailable)
{
	ReplaceData(Data.WarpAvailable, NewWarpAvailable, 33);
}

void AuxShipIndex::SetWarpTriggerTime(u32 NewWarpTriggerTime)
{
	ReplaceData(Data.WarpTriggerTime, NewWarpTriggerTime, 34);
}

void AuxShipIndex::SetTradeMoney(u64 NewTradeMoney)
{
	ReplaceData(Data.TradeMoney, NewTradeMoney, 43);
}

void AuxShipIndex::SetEngineThrustState(u32 NewEngineThrustState)
{
	ReplaceData(Data.EngineThrustState, NewEngineThrustState, 46);
}

void AuxShipIndex::SetEngineTrailType(u32 NewEngineTrailType)
{
	ReplaceData(Data.EngineTrailType, NewEngineTrailType, 47);
}

void AuxShipIndex::SetGuildName(char * NewGuildName)
{
	ReplaceString(Data.GuildName, NewGuildName, 48);
}

void AuxShipIndex::SetGuildRank(u32 NewGuildRank)
{
	ReplaceData(Data.GuildRank, NewGuildRank, 49);
}

void AuxShipIndex::SetGuildRankName(char * NewGuildRankName)
{
	ReplaceString(Data.GuildRankName, NewGuildRankName, 50);
}

void AuxShipIndex::SetSameGuildTagColor(float * NewSameGuildTagColor)
{
	ReplaceColor(Data.SameGuildTagColor, NewSameGuildTagColor, 51);
}

void AuxShipIndex::SetOtherGuildTagColor(float * NewOtherGuildTagColor)
{
	ReplaceColor(Data.OtherGuildTagColor, NewOtherGuildTagColor, 52);
}

void AuxShipIndex::SetInterruptAbilityName(char * NewInterruptAbilityName)
{
	ReplaceString(Data.InterruptAbilityName, NewInterruptAbilityName, 53);
}

void AuxShipIndex::SetInterruptState(u32 NewInterruptState)
{
	ReplaceData(Data.InterruptState, NewInterruptState, 54);
}

void AuxShipIndex::SetInterruptActivationTime(u32 NewInterruptActivationTime)
{
	ReplaceData(Data.InterruptActivationTime, NewInterruptActivationTime, 55);
}

void AuxShipIndex::SetInterruptProgress(float NewInterruptProgress)
{
	ReplaceData(Data.InterruptProgress, NewInterruptProgress, 56);
}

void AuxShipIndex::SetFactionIdentifier(const char * NewFactionIdentifier)
{
	ReplaceString(Data.FactionIdentifier, NewFactionIdentifier, 57);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxShipIndex::Reset()
{
	memset(Flags,0, sizeof(Flags));

	GameID = 0;

	*Data.Name = 0;
	*Data.Title = 0;
	*Data.Owner = 0;
	*Data.Rank = 0;
	Energy.Init(4, this, &Data.Energy);
	Data.MaxEnergy = 0;
	Shield.Init(6, this, &Data.Shield);
	Data.MaxShield = 0;
	Data.HullPoints = 0;
	Data.MaxHullPoints = 0;
	Data.MaxTiltRate = 0;
	Data.MaxTurnRate = 0;
	Data.MaxTiltAngle = 0;
	Data.MaxSpeed = 0;
	Data.MinSpeed = 0;
	Data.Acceleration = 0;
	Data.LockSpeed = 0;
	Data.LockOrient = 0;
	Data.AutoLevel = 0;
	Data.IsCloaked = 0;
	Data.IsCountermeasureActive = 0;
	Data.IsIncapacitated = 0;
	Data.IsOrganic = 0;
	Data.IsInPVP = 0;
	Data.IsAutoFollowing = 0;
	Data.IsRescueBeaconActive = 0;
	Data.CombatLevel = 0;
	Data.TargetGameID = 0;
	*Data.TargetThreat = 0;
	*Data.TargetThreatSound = 0;
	Data.TargetThreatLevel = 0;
	Data.PrivateWarpState = 0;
	Data.GlobalWarpState = 0;
	Data.WarpAvailable = 0;
	Data.WarpTriggerTime = 0;
	Shake.Init(35, this, &Data.Shake);
	Inventory.Init(36, this, &Data.Inventory);
	QuadrantDamage.Init(37, this, &Data.QuadrantDamage);
	DamageSpot.Init(38, this, &Data.DamageSpot);
	DamageLine.Init(39, this, &Data.DamageLine);
	DamageBlotch.Init(40, this, &Data.DamageBlotch);
	Lego.Init(41, this, &Data.Lego);
	Buffs.Init(42, this, &Data.Buffs);
	Data.TradeMoney = 0;
	BaseStats.Init(44, this, &Data.BaseStats);
	CurrentStats.Init(45, this, &Data.CurrentStats);
	Data.EngineThrustState = 0;
	Data.EngineTrailType = 0;
	*Data.GuildName = 0;
	Data.GuildRank = 0;
	*Data.GuildRankName = 0;
	Data.SameGuildTagColor[0] = 0;
	Data.SameGuildTagColor[1] = 0;
	Data.SameGuildTagColor[2] = 0;
	Data.OtherGuildTagColor[0] = 0;
	Data.OtherGuildTagColor[1] = 0;
	Data.OtherGuildTagColor[2] = 0;
	*Data.InterruptAbilityName = 0;
	Data.InterruptState = 0;
	Data.InterruptActivationTime = 0;
	Data.InterruptProgress = 0;
	*Data.FactionIdentifier = 0;

	ExtendedFlags[0] = char(0x06);
	ExtendedFlags[1] = char(0x05);
	ExtendedFlags[2] = char(0x00);
	ExtendedFlags[3] = char(0x00);
	ExtendedFlags[4] = char(0x00);
	ExtendedFlags[5] = char(0x21);
	ExtendedFlags[6] = char(0x03);
	ExtendedFlags[7] = char(0xC0);
	ExtendedFlags[8] = char(0xFF);
	ExtendedFlags[9] = char(0xFF);
	ExtendedFlags[10] = char(0xFF);
	ExtendedFlags[11] = char(0xFF);
	ExtendedFlags[12] = char(0xFF);
	ExtendedFlags[13] = char(0xFF);
	ExtendedFlags[14] = char(0xFE);
}

void AuxShipIndex::ClearFlags()
{
	memset(Flags,0, sizeof(Flags));
	Energy.ClearFlags();
	Shield.ClearFlags();
	Shake.ClearFlags();
	Inventory.ClearFlags();
	QuadrantDamage.ClearFlags();
	DamageSpot.ClearFlags();
	DamageLine.ClearFlags();
	DamageBlotch.ClearFlags();
	Lego.ClearFlags();
	Buffs.ClearFlags();
	BaseStats.ClearFlags();
	CurrentStats.ClearFlags();
}
