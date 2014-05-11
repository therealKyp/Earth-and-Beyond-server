##############################################################################
##
##  Detour of HeapAlloc
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##
##  Copyright 1995-2001, Microsoft Corporation
##


CLIB=/MT
!include ..\common.mak

all: dirs \
	"$(BIND)\heapa.dll" \
	"$(BIND)\heapa.bsc"

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\heapa.*" 2>nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\heapa.dll" "$(BIND)\heapa.lib" : heapa.cpp
	cl /LD $(CFLAGS) /Fe$@ "/Fdheapa.pdb" "heapa.cpp" \
		/link /incremental:no /subsystem:console \
		/entry:_DllMainCRTStartup@12 \
		/export:My_HeapAlloc \
		$(LIBS)

"$(BIND)\heapa.bsc" : heapa.sbr
	bscmake /v /n /o $@ heapa.sbr

##############################################################################
