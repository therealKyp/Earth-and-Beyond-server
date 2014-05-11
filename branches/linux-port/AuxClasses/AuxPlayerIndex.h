#ifndef _AUXPLAYERINDEX_H_INCLUDED_
#define _AUXPLAYERINDEX_H_INCLUDED_

#include "AuxSecureInventory.h"
#include "AuxVendorInventory.h"
#include "AuxRewardInventory.h"
#include "AuxOverflowInventory.h"
#include "AuxRPGInfo.h"
#include "AuxMissions.h"
#include "AuxReputation.h"
#include "AuxGroupInfo.h"

struct _PlayerIndex
{
	u64 Credits;
	u32 XPDebt;
	_SecureInv SecureInv;
	_VendorInv VendorInv;
	_RewardInv RewardInv;
	_OverflowInv OverflowInv;
	_RPGInfo RPGInfo;
	char CommunityEventFlags[64];
	u32 MusicID;
	_Missions Missions;
	_Reputation Reputation;
	u32 PIPAvatarID;
	char RegistrationStarbase[64];
	char RegistrationStarbaseSector[64];
	char SectorName[64];
	u32 SectorNum;
	u32 ClientSendUITriggers;
	_GroupInfo GroupInfo;
} ATTRIB_PACKED;

class AuxPlayerIndex : public AuxBase
{
public:
    AuxPlayerIndex()
	{
        Construct(Flags, ExtendedFlags, 18, 0, 0);
		memset(PacketBuffer, 0, sizeof(PacketBuffer));
		PacketSize = 0;
        m_Mutex = new Mutex();

        Reset();
	}

    ~AuxPlayerIndex()
	{
        if (m_Mutex)
            delete m_Mutex;
	}

    void Reset();
    void ClearFlags();

	bool BuildPacket();
	bool BuildPacket(unsigned char *, long &);

	bool BuildExtendedPacket();
	bool BuildExtendedPacket(unsigned char *, long &);

	_PlayerIndex * GetData();

	u64 GetCredits();
	u32 GetXPDebt();
	char * GetCommunityEventFlags();
	u32 GetMusicID();
	u32 GetPIPAvatarID();
	char * GetRegistrationStarbase();
	char * GetRegistrationStarbaseSector();
	char * GetSectorName();
	u32 GetSectorNum();
	u32 GetClientSendUITriggers();

	void SetData(_PlayerIndex *);

	void SetCredits(u64);
	void SetXPDebt(u32);
	void SetCommunityEventFlags(char *);
	void SetMusicID(u32);
	void SetPIPAvatarID(u32);
	void SetRegistrationStarbase(const char *);
	void SetRegistrationStarbaseSector(const char *);
	void SetSectorName(char *);
	void SetSectorNum(u32);
	void SetClientSendUITriggers(u32);


private:
	_PlayerIndex Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[5];

public:
	unsigned char PacketBuffer[10000];
	long PacketSize;

	class AuxSecureInv SecureInv;
	class AuxVendorInv VendorInv;
	class AuxRewardInv RewardInv;
	class AuxOverflowInv OverflowInv;
	class AuxRPGInfo RPGInfo;
	class AuxMissions Missions;
	class AuxReputation Reputation;
	class AuxGroupInfo GroupInfo;
};

#endif
