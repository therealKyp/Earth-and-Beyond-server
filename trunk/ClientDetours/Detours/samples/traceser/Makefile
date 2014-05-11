######################################################################
##
##  Utility to trace serial (COM1, COM2, etc.) APIs.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##

CLIB=/MT
!include ..\common.mak

all: dirs \
	"$(BIND)\traceser.dll" \
	"$(BIND)\traceser.bsc"

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\traceser.dll": traceser.cpp
	cl /LD $(CFLAGS) /Fe$@ /Fd"$(BIND)\traceser.pdb" "traceser.cpp" \
		/link /incremental:no /subsystem:console \
		/entry:_DllMainCRTStartup@12 /export:NullExport \
		$(LIBS)

"$(BIND)\traceser.bsc" : traceser.sbr
	bscmake /v /n /o $@ traceser.sbr

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\traceser.*" 2>nul

################################################################# End of File.
