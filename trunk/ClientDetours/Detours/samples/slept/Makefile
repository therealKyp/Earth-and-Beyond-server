######################################################################
##
##  API Extention to Measure time slept.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##
##  Copyright 1995-2001, Microsoft Corporation
##

CLIB=/MT
!include ..\common.mak

##############################################################################

all: dirs \
	"$(BIND)\slept.dll"		\
	"$(BIND)\sleepold.exe"	\
	"$(BIND)\sleepnew.exe"	\
	"$(BIND)\sleepinj.exe"	\
	\
	"$(BIND)\slept.bsc"		\
	"$(BIND)\sleepold.bsc"	\
	"$(BIND)\sleepnew.bsc"	\
	"$(BIND)\sleepinj.bsc"	\

##############################################################################

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\slept.dll" "$(BIND)\slept.lib": slept.cpp verify.cpp
	cl /LD $(CFLAGS) /Fe$@ "/Fd$(BIND)\slept.pdb" "slept.cpp" \
		/link /incremental:no /subsystem:console \
		/entry:_DllMainCRTStartup@12 \
		/export:TimedSleep \
		/export:UntimedSleep \
		/export:GetSleptTicks \
		$(LIBS)

"$(BIND)\slept.bsc" : slept.sbr
	bscmake /v /n /o $@ slept.sbr

"$(BIND)\sleepold.exe" : sleepold.cpp verify.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\sleepold.pdb" sleepold.cpp \
		/link $(LIBS) \
		/subsystem:console /incremental:no

"$(BIND)\sleepold.bsc" : sleepold.sbr
	bscmake /v /n /o $@ sleepold.sbr

"$(BIND)\sleepnew.exe" : sleepnew.cpp "$(BIND)\slept.lib" verify.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\sleepnew.pdb" sleepnew.cpp \
		/link $(LIBS) \
		/subsystem:console /incremental:no "$(BIND)\slept.lib"

"$(BIND)\sleepnew.bsc" : sleepnew.sbr
	bscmake /v /n /o $@ sleepnew.sbr

"$(BIND)\sleepinj.exe" : sleepinj.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\sleepinj.pdb" sleepinj.cpp \
		/link $(LIBS) \
		/subsystem:console /incremental:no

"$(BIND)\sleepinj.bsc" : sleepinj.sbr
	bscmake /v /n /o $@ sleepinj.sbr

##############################################################################

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\slept.*" 2>nul
	-del "$(BIND)\sleepold.*" 2>nul
	-del "$(BIND)\sleepnew.*" 2>nul
	-del "$(BIND)\sleepinj.*" 2>nul

##############################################################################
