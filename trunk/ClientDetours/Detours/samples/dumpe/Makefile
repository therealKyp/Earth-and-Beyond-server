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
	"$(BIND)\dumpe.exe" \
	"$(BIND)\dumpe.bsc"

clean:
	-del *.obj *.sbr *~ 2>nul
	-del "$(BIND)\dumpe.*" 2>nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\dumpe.exe" : dumpe.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\dumpe.pdb" dumpe.cpp \
		/link $(LIBS) \
		/subsystem:console /incremental:no

"$(BIND)\dumpe.bsc" : dumpe.sbr
	bscmake /v /n /o $@ dumpe.sbr

################################################################# End of File
