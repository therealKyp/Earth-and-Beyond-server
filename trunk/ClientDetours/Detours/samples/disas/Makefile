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
	"$(BIND)\disas.exe" \
	"$(BIND)\disas.bsc"

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\disas.*" 2> nul 

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\disas.exe" : disas.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\disas.pdb" disas.cpp \
		/link $(LIBS) /subsystem:console /incremental:no /entry:WinMainCRTStartup

"$(BIND)\disas.bsc" : disas.sbr
	bscmake /v /n /o $@ disas.sbr

################################################################# End of File.
