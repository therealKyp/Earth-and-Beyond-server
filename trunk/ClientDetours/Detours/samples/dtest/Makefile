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
	"$(BIND)\dtest.exe" \
	"$(BIND)\dtest.bsc"

clean:
	-del *~ *.obj *.sbr 2> nul
	-del "$(BIND)\dtest.*" 2> nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\dtest.exe" : dtest.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\dtest.pdb" dtest.cpp \
		/link $(LIBS) /subsystem:console /incremental:no /entry:WinMainCRTStartup

"$(BIND)\dtest.bsc" : dtest.sbr
	bscmake /v /n /o $@ dtest.sbr

################################################################# End of File.
