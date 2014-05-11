##############################################################################
##
##  Utility to trace Dynamic Linking.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##

CLIB=/MT
!include ..\common.mak
LIBS=$(LIBS)

##############################################################################

all: dirs \
	"$(BIND)\tracelnk.dll" \
	"$(BIND)\tracelnk.bsc"

##############################################################################

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\tracelnk.*" 2>nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\tracelnk.dll": tracelnk.cpp
	cl /LD $(CFLAGS) /Fe$@ /Fd"$(BIND)\tracelnk.pdb" "tracelnk.cpp" \
		/link /incremental:no /subsystem:console \
		/entry:_DllMainCRTStartup@12 /export:NullExport \
		$(LIBS)

"$(BIND)\tracelnk.bsc" : tracelnk.sbr
	bscmake /v /n /o $@ tracelnk.sbr

##############################################################################
