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

all: dirs	\
	"$(BIND)\withdll.exe"	\
	"$(BIND)\withdll.bsc"

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\withdll.*" 2>nul

##############################################################################

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\withdll.exe" : withdll.cpp
	cl $(CFLAGS) /Fe$@ "/Fd$(BIND)\withdll.pdb" withdll.cpp \
		/link $(LIBS) \
		/subsystem:console /incremental:no

"$(BIND)\withdll.bsc" : withdll.sbr
	bscmake /v /n /o $@ withdll.sbr

################################################################# End of File.
