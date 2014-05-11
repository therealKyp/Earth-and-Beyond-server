######################################################################
##
##  Utility to registry and file access APIs.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##

CLIB=/MT
!include ..\common.mak

LIBS=$(LIBS) advapi32.lib

all: dirs \
	"$(BIND)\tracereg.dll" \
	"$(BIND)\tracereg.bsc"

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\tracereg.dll": tracereg.cpp
	cl /LD $(CFLAGS) /Fe$@ /Fd"$(BIND)\tracereg.pdb" "tracereg.cpp" \
		/link /incremental:no /subsystem:console \
		/entry:_DllMainCRTStartup@12 /export:NullExport \
		$(LIBS)

"$(BIND)\tracereg.bsc" : tracereg.sbr
	bscmake /v /n /o $@ tracereg.sbr

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\tracereg.*" 2>nul

################################################################# End of File.
