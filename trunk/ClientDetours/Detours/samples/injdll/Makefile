##############################################################################
##
##  Makefile for Detours Test Programs.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##
##  Copyright 1995-2001, Microsoft Corporation
##

!include ..\common.mak

##############################################################################

all: dirs \
	"$(BIND)\injdll.exe" \
	"$(BIND)\injdll.bsc"

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\injdll.*" 2>nul

##############################################################################

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\injdll.exe" : injdll.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\injdll.pdb" "injdll.cpp" \
		/link $(LIBS) \
		/subsystem:console /incremental:no

"$(BIND)\injdll.bsc" : injdll.sbr
	bscmake /v /n /o $@ injdll.sbr

################################################################# End of File.
