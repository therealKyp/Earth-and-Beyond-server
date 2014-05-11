##############################################################################
##
##  Makefile for Detours Test Programs.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##
##  Copyright 1995-2001, Microsoft Corporation
##

!include ..\common.mak

all: dirs \
	"$(BIND)\excep.exe" \
	"$(BIND)\excep.bsc"

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\excep.*" 2>nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\excep.exe" : excep.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\excep.pdb" excep.cpp \
		/link $(LIBS) /subsystem:console /incremental:no /entry:WinMainCRTStartup

"$(BIND)\excep.bsc" : excep.sbr
	bscmake /v /n /o $@ excep.sbr

################################################################# End of File.
