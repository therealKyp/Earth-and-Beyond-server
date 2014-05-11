##############################################################################
##
##  Makefile for Detours Test Programs.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##
##  Copyright 1995-2001, Microsoft Corporation
##

CLIB=/MT
!include ..\common.mak

all: dirs \
	"$(BIND)\edll1.dll"	\
	"$(BIND)\edll2.dll"	\
	"$(BIND)\edll3.dll"	\
	"$(BIND)\einst.exe"	\
	\
	"$(BIND)\edll1.bsc"	\
	"$(BIND)\edll2.bsc"	\
	"$(BIND)\edll3.bsc"	\
	"$(BIND)\einst.bsc"	\

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\edll1.*" 2>nul
	-del "$(BIND)\edll2.*" 2>nul
	-del "$(BIND)\edll3.*" 2>nul
	-del "$(BIND)\einst.*" 2>nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\einst.exe" : einst.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\einst.pdb" einst.cpp \
		/link $(LIBS) \
		"$(BIND)\edll1.lib" "$(BIND)\edll2.lib" "$(BIND)\edll3.lib" \
		/subsystem:console /entry:WinMainCRTStartup /incremental:no

"$(BIND)\einst.bsc" : einst.sbr
	bscmake /v /n /o $@ einst.sbr

"$(BIND)\edll1.dll" : edll1.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\edll1.pdb" edll1.cpp /LDd \
		/link $(LIBS) \
		/subsystem:windows /entry:_DllMainCRTStartup@12 /incremental:no \
		/base:0x7100000

"$(BIND)\edll1.bsc" : edll1.sbr
	bscmake /v /n /o $@ edll1.sbr

"$(BIND)\edll2.dll" : edll2.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\edll2.pdb" edll2.cpp /LDd \
		/link $(LIBS) \
		/subsystem:console /entry:_DllMainCRTStartup@12 /incremental:no \
		/base:0x7200000

"$(BIND)\edll2.bsc" : edll2.sbr
	bscmake /v /n /o $@ edll2.sbr

"$(BIND)\edll3.dll" : edll3.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\edll3.pdb" edll3.cpp /LDd \
		/link $(LIBS) \
		/subsystem:console /entry:_DllMainCRTStartup@12 /incremental:no \
		/base:0x7300000

"$(BIND)\edll3.bsc" : edll3.sbr
	bscmake /v /n /o $@ edll3.sbr

################################################################# End of File.
