##############################################################################
##
##  Utility to trace Win32 APIs.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##

CLIB=/MT
!include ..\common.mak
LIBS=$(LIBS) advapi32.lib ole32.lib ws2_32.lib

#LIBS = kernel32.lib uuid.lib advapi32.lib user32.lib ole32.lib detours.lib \
#	gdi32.lib glu32.lib shell32.lib comdlg32.lib mpr.lib  \
#	winspool.lib version.lib urlmon.lib winmm.lib opengl32.lib msvcrt.lib \
#	imagehlp.lib

##############################################################################

all: dirs \
	"$(BIND)\traceapi.dll" \
	"$(BIND)\traceapi.bsc"

##############################################################################

clean:
	-del *~ *.obj *.sbr 2>nul
	-del "$(BIND)\traceapi.*" 2>nul

dirs:
	if not exist "$(BIND)" mkdir "$(BIND)"

"$(BIND)\traceapi.dll": traceapi.cpp _win32.cpp
	cl /LD $(CFLAGS) /Fe$@ /Fd"$(BIND)\traceapi.pdb" "traceapi.cpp" \
		/link /incremental:no /subsystem:console \
		/entry:_DllMainCRTStartup@12 /export:NullExport \
		$(LIBS)

"$(BIND)\traceapi.bsc" : traceapi.sbr
	bscmake /v /n /o $@ traceapi.sbr

##############################################################################
