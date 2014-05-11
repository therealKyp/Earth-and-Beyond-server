##############################################################################
##
##  Makefile for Detours.
##
##  Microsoft Research Detours Package.  Version 1.5 (Build 46)
##
##  Copyright 1995-2001, Microsoft Corporation
##
##############################################################################

TARGETOS=WINNT
!include ..\common.mak

##############################################################################
##
.SUFFIXES: .cpp .h .obj
.cpp.obj:
	@$(CC) /nologo $(CFLAGS) "/Fo$*.obj" /c "$*.cpp"

##############################################################################
##
LFLAGS=/link /incremental:no

##############################################################################

all: dirs \
	"$(INCD)\syelog.h"		\
	"$(LIBD)\syelog.lib"	\
	"$(BIND)\syelogd.exe"	\
	\
	"$(BIND)\sltest.exe" 	\
	"$(BIND)\sltests.exe" 	\
	"$(BIND)\sltestp.exe" 	\
	"$(BIND)\slteste.exe" 	\
	\
	"$(BIND)\syelogd.bsc"	\
	"$(BIND)\sltest.bsc" 	\
	"$(BIND)\sltests.bsc" 	\
	"$(BIND)\sltestp.bsc" 	\
	"$(BIND)\slteste.bsc" 	\

##############################################################################
##
clean:
	-del *.obj *.sbr *~ 2> nul
	-del "$(INCD)\syelog.*" 2>nul
	-del "$(LIBD)\syelog.*" 2>nul
	-del "$(BIND)\syelogd.*" 2>nul
	-del "$(BIND)\sltest.*" 2>nul
	-del "$(BIND)\sltests.*" 2>nul
	-del "$(BIND)\sltestp.*" 2>nul
	-del "$(BIND)\slteste.*" 2>nul

##############################################################################

dirs:
	if not exist "$(INCD)" mkdir "$(INCD)"
	if not exist "$(LIBD)" mkdir "$(LIBD)"
	if not exist "$(BIND)" mkdir "$(BIND)"

syelog.obj : syelog.cpp syelog.h
    $(CC) $(CFLAGS) /Zl "/Fd$(LIBD)\syelog.pdb" /c syelog.cpp

"$(INCD)\syelog.h" : syelog.h
	copy syelog.h $@

"$(LIBD)\syelog.lib" : syelog.obj
	lib /debugtype:cv /nologo /out:$@ syelog.obj

"$(BIND)\sltest.exe": sltest.cpp syelog.h syelog.cpp
    $(CC) $(CFLAGS) /Fe$@ "/Fd$(BIND)\sltest.pdb" sltest.cpp \
		/link /incremental:no $(LIBS)

"$(BIND)\sltest.bsc" : sltest.sbr
	bscmake /v /n /o $@ sltest.sbr

"$(BIND)\sltests.exe": sltests.cpp syelog.h
    $(CC) $(CFLAGS) /Fe$@ "/Fd$(BIND)\sltests.pdb" sltests.cpp \
		/link /incremental:no ws2_32.lib mswsock.lib

"$(BIND)\sltests.bsc" : sltests.sbr
	bscmake /v /n /o $@ sltests.sbr

"$(BIND)\sltestp.exe": sltestp.cpp syelog.h
    $(CC) $(CFLAGS) /Fe$@ "/Fd$(BIND)\sltestp.pdb" sltestp.cpp \
		/link /incremental:no

"$(BIND)\sltestp.bsc" : sltestp.sbr
	bscmake /v /n /o $@ sltestp.sbr

"$(BIND)\slteste.exe": slteste.cpp syelog.h
    $(CC) $(CFLAGS) /Fe$@ "/Fd$(BIND)\slteste.pdb" slteste.cpp \
		/link /incremental:no advapi32.lib

"$(BIND)\slteste.bsc" : slteste.sbr
	bscmake /v /n /o $@ slteste.sbr

"$(BIND)\syelogd.exe": syelogd.cpp syelog.h
    $(CC) $(CFLAGS) /Fe$@ "/Fd$(BIND)\syelogd.pdb" syelogd.cpp \
		/link /incremental:no ws2_32.lib mswsock.lib advapi32.lib

"$(BIND)\syelogd.bsc" : syelogd.sbr
	bscmake /v /n /o $@ syelogd.sbr

################################################################# End of File.
