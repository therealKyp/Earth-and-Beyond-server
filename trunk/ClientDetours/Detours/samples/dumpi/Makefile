##############################################################################
##
##  Makefile for Detours Test Programs - Dump Imports
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##
##  Copyright 1995-2001, Microsoft Corporation
##

!include ..\common.mak

all: dirs \
	"$(BIND)\dumpi.exe" \
	"$(BIND)\dumpi.bsc" \

clean:
	-del *.obj *.sbr *~ 2>nul
	-del "$(BIND)\dumpi.*" 2>nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\dumpi.exe" : dumpi.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\dumpi.pdb" dumpi.cpp \
		/link $(LIBS) \
		/subsystem:console /incremental:no

"$(BIND)\dumpi.bsc" : dumpi.sbr
	bscmake /v /n /o $@ dumpi.sbr

################################################################# End of File.
