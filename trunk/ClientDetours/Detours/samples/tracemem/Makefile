##############################################################################
##
##  Utility to trace HeapAlloc APIs.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##

CLIB=/MT
!include ..\common.mak

all: dirs \
	"$(BIND)\tracemem.dll" \
	"$(BIND)\tracemem.bsc"

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\tracemem.*" 2>nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\tracemem.dll": tracemem.cpp
	cl /LD $(CFLAGS) /Fe$@ /Fd"$(BIND)\tracemem.pdb" "tracemem.cpp" \
		/link /incremental:no /subsystem:console \
		/entry:_DllMainCRTStartup@12 /export:NullExport \
		$(LIBS)

"$(BIND)\tracemem.bsc" : tracemem.sbr
	bscmake /v /n /o $@ tracemem.sbr

################################################################# End of File.
