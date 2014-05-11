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
	"$(BIND)\setdll.exe"	\
	"$(BIND)\setdll.bsc"

##############################################################################

clean:
	-del *.obj *.sbr *~ 2>nul
	-del "$(BIND)\setdll.*" 2>nul

##############################################################################

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\setdll.exe" : setdll.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\setdll.pdb" setdll.cpp \
		/link $(LIBS) /subsystem:console /incremental:no

"$(BIND)\setdll.bsc" : setdll.sbr
	bscmake /v /n /o $@ setdll.sbr

##############################################################################
# End of File
