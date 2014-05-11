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
	"$(BIND)\member.exe" \
	"$(BIND)\member.bsc"

clean:
	-del *~ *.obj *.sbr 2> nul
	-del "$(BIND)\member.*" 2> nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\member.exe" : member.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\member.pdb" member.cpp \
		/link $(LIBS) /subsystem:console /incremental:no

"$(BIND)\member.bsc" : member.sbr
	bscmake /v /n /o $@ member.sbr

################################################################# End of File.
