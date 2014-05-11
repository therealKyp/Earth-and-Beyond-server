# Microsoft Developer Studio Project File - Name="Net7" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Net7 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Net7.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Net7.mak" CFG="Net7 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Net7 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Net7 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Net7 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "USE_OPENSSL" /Fr /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"C:\net7\bin\Net7.exe" /libpath:"../libs"
# SUBTRACT LINK32 /incremental:yes /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "Net7 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "USE_OPENSSL" /FAs /FR /FD /GZ /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /stack:0x1e8480 /subsystem:console /debug /machine:I386 /out:"C:\net7\bin\Net7.exe" /libpath:"../libs"
# SUBTRACT LINK32 /pdb:none /map /nodefaultlib

!ENDIF 

# Begin Target

# Name "Net7 - Win32 Release"
# Name "Net7 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "SQL Loaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ItemBaseSQL.cpp
# End Source File
# Begin Source File

SOURCE=.\MOBDatabaseSQL.cpp
# End Source File
# Begin Source File

SOURCE=.\SectorContentSQL.cpp
# End Source File
# Begin Source File

SOURCE=.\StationLoader.cpp
# End Source File
# End Group
# Begin Group "XML Parser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ItemBaseParser.cpp
# End Source File
# Begin Source File

SOURCE=.\MissionParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SectorContentParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillParser.cpp
# End Source File
# Begin Source File

SOURCE=.\TalkTreeParser.cpp
# End Source File
# Begin Source File

SOURCE=.\XmlParser.cpp
# End Source File
# End Group
# Begin Group "ObjectClassSrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FieldClass.cpp
# End Source File
# Begin Source File

SOURCE=.\MOBClass.cpp
# End Source File
# Begin Source File

SOURCE=.\MOBSpawnClass.cpp
# End Source File
# Begin Source File

SOURCE=.\NavTypeClass.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectClass.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectClassMovement.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerAbilitys.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerClass.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerCombat.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerManufacturing.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerMissions.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerSaves.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerSkills.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceClass.cpp
# End Source File
# End Group
# Begin Group "AuxClasses"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AuxClasses\AuxAttachment.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxAttachment.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxAttachments.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxAttachments.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxBase.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxBase.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxBuff.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxBuff.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxBuffs.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxBuffs.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxCategories.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxCategories.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxCategory.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxCategory.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxDamage.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxDamage.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxEffect.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxElement.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxElement.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxElements.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxElements.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxEquipInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxEquipInventory.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxEquipItem.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxEquipItem.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxFaction.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxFaction.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxFactions.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxFactions.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxGroupInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxGroupInfo.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxGroupMember.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxGroupMember.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxGroupMembers.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxGroupMembers.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxHarvestable.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxHarvestable.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxHulkIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxHulkIndex.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxInventory1.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxInventory1.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxInventory20.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxInventory20.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxInventory40.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxInventory40.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxInventory6.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxInventory6.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxItem.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxItem.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxKnownFormula.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxKnownFormula.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxKnownFormulas.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxKnownFormulas.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxLego.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxLego.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxManufacturingIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxManufacturingIndex.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMission.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMission.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMissions.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMissions.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMissionStage.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMissionStage.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMissionStages.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMissionStages.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMobIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMobIndex.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMountBoneNames.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMountBoneNames.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMounts.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxMounts.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxOverflowInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxOverflowInventory.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPercent.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPercent.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPlayerIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPlayerIndex.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPreviousAttempts.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPreviousAttempts.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPrimaryCategories.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPrimaryCategories.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPrimaryCategory.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxPrimaryCategory.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxQuadrantDamage.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxQuadrantDamage.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxReputation.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxReputation.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxRewardInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxRewardInventory.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxRPGInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxRPGInfo.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSecureInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSecureInventory.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxShake.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxShake.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxShipIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxShipIndex.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxShipInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxShipInventory.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSkill.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSkillAbilities.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSkillAbilities.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSkillAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSkillAbility.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSkills.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSkills.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxStats.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxStats.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSubCategories.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSubCategories.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSubCategory.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxSubCategory.h
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxVendorInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\AuxClasses\AuxVendorInventory.h
# End Source File
# End Group
# Begin Group "xmlParser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xmlParser\xmlParser.h
# End Source File
# Begin Source File

SOURCE=.\xmlParser\xmlParser_.cpp
# End Source File
# End Group
# Begin Group "mySql"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mysql\mysqlplus.cpp

!IF  "$(CFG)" == "Net7 - Win32 Release"

!ELSEIF  "$(CFG)" == "Net7 - Win32 Debug"

# ADD CPP /D "_WIN32"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mysql\mysqlplus.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AccountManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientToGlobalServer.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientToMasterServer.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientToSectorServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Connection.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Equipable.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemBaseManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\MasterServerToSectorServer.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\MissionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\Net7.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SectorManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SectorServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SectorServerToSectorServer.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SSL_Connection.cpp
# End Source File
# Begin Source File

SOURCE=.\SSL_Listener.cpp
# End Source File
# Begin Source File

SOURCE=.\Stats.cpp
# End Source File
# Begin Source File

SOURCE=.\StringManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpListener.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP_Client.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP_Global.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP_Master.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP_MVAS.cpp
# End Source File
# Begin Source File

SOURCE=.\UDPConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\WestwoodRC4.cpp
# End Source File
# Begin Source File

SOURCE=.\WestwoodRSA.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "SQL Loaders Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MOBDatabase.h
# End Source File
# Begin Source File

SOURCE=.\StationLoader.h
# End Source File
# End Group
# Begin Group "XML Parser Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ItemBaseParser.h
# End Source File
# Begin Source File

SOURCE=.\MissionParser.h
# End Source File
# Begin Source File

SOURCE=.\SectorContentParser.h
# End Source File
# Begin Source File

SOURCE=.\SkillParser.h
# End Source File
# Begin Source File

SOURCE=.\TalkTree.h
# End Source File
# Begin Source File

SOURCE=.\TalkTreeParser.h
# End Source File
# Begin Source File

SOURCE=.\XmlParser.h
# End Source File
# Begin Source File

SOURCE=.\XmlTags.h
# End Source File
# End Group
# Begin Group "ObjectClassHeader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FieldClass.h
# End Source File
# Begin Source File

SOURCE=.\MOBClass.h
# End Source File
# Begin Source File

SOURCE=.\MOBSpawnClass.h
# End Source File
# Begin Source File

SOURCE=.\NavTypeClass.h
# End Source File
# Begin Source File

SOURCE=.\ObjectClass.h
# End Source File
# Begin Source File

SOURCE=.\ObjectManager.h
# End Source File
# Begin Source File

SOURCE=.\PlayerClass.h
# End Source File
# Begin Source File

SOURCE=.\PlayerManufacturing.h
# End Source File
# Begin Source File

SOURCE=.\PlayerSkills.h
# End Source File
# Begin Source File

SOURCE=.\ResourceClass.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AccountManager.h
# End Source File
# Begin Source File

SOURCE=.\Connection.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager.h
# End Source File
# Begin Source File

SOURCE=.\Equipable.h
# End Source File
# Begin Source File

SOURCE=.\ItemBase.h
# End Source File
# Begin Source File

SOURCE=.\ItemBaseManager.h
# End Source File
# Begin Source File

SOURCE=.\ItemList.h
# End Source File
# Begin Source File

SOURCE=.\MemoryHandler.h
# End Source File
# Begin Source File

SOURCE=.\MessageQueue.h
# End Source File
# Begin Source File

SOURCE=.\MissionManager.h
# End Source File
# Begin Source File

SOURCE=.\Mutex.h
# End Source File
# Begin Source File

SOURCE=.\Net7.h
# End Source File
# Begin Source File

SOURCE=.\Opcodes.h
# End Source File
# Begin Source File

SOURCE=.\PacketMethods.h
# End Source File
# Begin Source File

SOURCE=.\PacketStructures.h
# End Source File
# Begin Source File

SOURCE=.\PlayerManager.h
# End Source File
# Begin Source File

SOURCE=.\SectorData.h
# End Source File
# Begin Source File

SOURCE=.\SectorManager.h
# End Source File
# Begin Source File

SOURCE=.\SectorServerManager.h
# End Source File
# Begin Source File

SOURCE=.\ServerManager.h
# End Source File
# Begin Source File

SOURCE=.\SSL_Connection.h
# End Source File
# Begin Source File

SOURCE=.\SSL_Listener.h
# End Source File
# Begin Source File

SOURCE=.\StaticData.h
# End Source File
# Begin Source File

SOURCE=.\Stats.h
# End Source File
# Begin Source File

SOURCE=.\StringManager.h
# End Source File
# Begin Source File

SOURCE=.\TcpListener.h
# End Source File
# Begin Source File

SOURCE=.\TimeNode.h
# End Source File
# Begin Source File

SOURCE=.\UDPConnection.h
# End Source File
# Begin Source File

SOURCE=.\WestwoodRC4.h
# End Source File
# Begin Source File

SOURCE=.\WestwoodRSA.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
