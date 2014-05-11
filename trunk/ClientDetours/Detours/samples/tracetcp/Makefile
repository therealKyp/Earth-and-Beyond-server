######################################################################
##
##  Utility to trace WinSock TCP APIs.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##

CLIB=/MT
!include ..\common.mak

LIBS=$(LIBS) ws2_32.lib

##############################################################################

all: dirs \
	"$(BIND)\tracetcp.dll" \
	"$(BIND)\tracetcp.bsc"

##############################################################################

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\tracetcp.dll": tracetcp.cpp
	cl /LD $(CFLAGS) /Fe$@ /Fd"$(BIND)\tracetcp.pdb" "tracetcp.cpp" \
		/link /incremental:no /subsystem:console \
		/entry:_DllMainCRTStartup@12 /export:NullExport \
		$(LIBS)

"$(BIND)\tracetcp.bsc" : tracetcp.sbr
	bscmake /v /n /o $@ tracetcp.sbr

##############################################################################

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\tracetcp.*" 2>nul

##############################################################################
