# Microsoft Developer Studio Generated NMAKE File, Based on Net7Mysql.dsp
!IF "$(CFG)" == ""
CFG=Net7Mysql - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Net7Mysql - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Net7Mysql - Win32 Release" && "$(CFG)" != "Net7Mysql - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Net7Mysql.mak" CFG="Net7Mysql - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Net7Mysql - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Net7Mysql - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Net7Mysql - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Net7Mysql.exe" "$(OUTDIR)\Net7Mysql.bsc"


CLEAN :
	-@erase "$(INTDIR)\mysqlplus.obj"
	-@erase "$(INTDIR)\mysqlplus.sbr"
	-@erase "$(INTDIR)\Net7Mysql.obj"
	-@erase "$(INTDIR)\Net7Mysql.pch"
	-@erase "$(INTDIR)\Net7Mysql.res"
	-@erase "$(INTDIR)\Net7Mysql.sbr"
	-@erase "$(INTDIR)\Net7MysqlDlg.obj"
	-@erase "$(INTDIR)\Net7MysqlDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Tab2.obj"
	-@erase "$(INTDIR)\Tab2.sbr"
	-@erase "$(INTDIR)\Tab3.obj"
	-@erase "$(INTDIR)\Tab3.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Net7Mysql.bsc"
	-@erase "$(OUTDIR)\Net7Mysql.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Net7Mysql.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Net7Mysql.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Net7Mysql.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\mysqlplus.sbr" \
	"$(INTDIR)\Net7Mysql.sbr" \
	"$(INTDIR)\Net7MysqlDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Tab2.sbr" \
	"$(INTDIR)\Tab3.sbr"

"$(OUTDIR)\Net7Mysql.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=libmySQL.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Net7Mysql.pdb" /machine:I386 /out:"$(OUTDIR)\Net7Mysql.exe" 
LINK32_OBJS= \
	"$(INTDIR)\mysqlplus.obj" \
	"$(INTDIR)\Net7Mysql.obj" \
	"$(INTDIR)\Net7MysqlDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tab2.obj" \
	"$(INTDIR)\Tab3.obj" \
	"$(INTDIR)\Net7Mysql.res"

"$(OUTDIR)\Net7Mysql.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Net7Mysql - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Net7Mysql.exe" "$(OUTDIR)\Net7Mysql.bsc"


CLEAN :
	-@erase "$(INTDIR)\mysqlplus.obj"
	-@erase "$(INTDIR)\mysqlplus.sbr"
	-@erase "$(INTDIR)\Net7Mysql.obj"
	-@erase "$(INTDIR)\Net7Mysql.pch"
	-@erase "$(INTDIR)\Net7Mysql.res"
	-@erase "$(INTDIR)\Net7Mysql.sbr"
	-@erase "$(INTDIR)\Net7MysqlDlg.obj"
	-@erase "$(INTDIR)\Net7MysqlDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Tab2.obj"
	-@erase "$(INTDIR)\Tab2.sbr"
	-@erase "$(INTDIR)\Tab3.obj"
	-@erase "$(INTDIR)\Tab3.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Net7Mysql.bsc"
	-@erase "$(OUTDIR)\Net7Mysql.exe"
	-@erase "$(OUTDIR)\Net7Mysql.ilk"
	-@erase "$(OUTDIR)\Net7Mysql.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Net7Mysql.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Net7Mysql.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Net7Mysql.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\mysqlplus.sbr" \
	"$(INTDIR)\Net7Mysql.sbr" \
	"$(INTDIR)\Net7MysqlDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Tab2.sbr" \
	"$(INTDIR)\Tab3.sbr"

"$(OUTDIR)\Net7Mysql.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=libmySQL.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Net7Mysql.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Net7Mysql.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\mysqlplus.obj" \
	"$(INTDIR)\Net7Mysql.obj" \
	"$(INTDIR)\Net7MysqlDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tab2.obj" \
	"$(INTDIR)\Tab3.obj" \
	"$(INTDIR)\Net7Mysql.res"

"$(OUTDIR)\Net7Mysql.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Net7Mysql.dep")
!INCLUDE "Net7Mysql.dep"
!ELSE 
!MESSAGE Warning: cannot find "Net7Mysql.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Net7Mysql - Win32 Release" || "$(CFG)" == "Net7Mysql - Win32 Debug"
SOURCE=.\mysql\mysqlplus.cpp

"$(INTDIR)\mysqlplus.obj"	"$(INTDIR)\mysqlplus.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Net7Mysql.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Net7Mysql.cpp

"$(INTDIR)\Net7Mysql.obj"	"$(INTDIR)\Net7Mysql.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Net7Mysql.pch"


SOURCE=.\Net7Mysql.rc

"$(INTDIR)\Net7Mysql.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Net7MysqlDlg.cpp

"$(INTDIR)\Net7MysqlDlg.obj"	"$(INTDIR)\Net7MysqlDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Net7Mysql.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Net7Mysql - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Net7Mysql.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Net7Mysql.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Net7Mysql - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Net7Mysql.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Net7Mysql.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Tab2.cpp

"$(INTDIR)\Tab2.obj"	"$(INTDIR)\Tab2.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Net7Mysql.pch"


SOURCE=.\Tab3.cpp

"$(INTDIR)\Tab3.obj"	"$(INTDIR)\Tab3.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Net7Mysql.pch"



!ENDIF 

