
!IFNDEF PGVER
!ERROR PGVER=PG94 or PGVER=PG82
!ENDIF

!IFNDEF CPU
!ERROR CPU=x86 or CPU=x64
!ENDIF

BINDIR = bin\$(PGVER)_$(CPU)
OBJDIR = obj\$(PGVER)_$(CPU)
!IF "$(CPU)" == "x86"
CXX = "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\cl.exe"
LINK = "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\link.exe"
LIB = C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib;C:\Program Files (x86)\Windows Kits\8.0\lib\win8\um\x86
!ELSEIF "$(CPU)" == "x64"
CXX = "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\x86_amd64\cl.exe"
LINK = "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\x86_amd64\link.exe"
LIB = C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib\amd64;C:\Program Files (x86)\Windows Kits\8.0\lib\win8\um\x64
!ELSE
!ERROR CPU Unknown. CPU=x86 or CPU=x64
!ENDIF
CXXFLAGS = /D$(PGVER) /DBUILDING_DLL /DWIN32 /DNDEBUG /D_WINDOWS /D_USRDLL /D_WINDLL /D_ATL_STATIC_REGISTRY /LD /MT /Ot /GL /DUNICODE /c /EHsc 
INCS = /I ../include /I $(PGVER)/include /I $(PGVER)/include/server /I hasphl/include
LIBS = /libpath:"$(LIB)" lib_$(CPU)/senna/libsenna.lib $(PGVER)/lib_$(CPU)/postgres.lib /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /MACHINE:$(CPU) /DLL /RELEASE 
# vsvars32等が定義する環境変数を上書き。特に、x86のLIBが厄介。
INCLUDE = include;C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\INCLUDE;C:\Program Files (x86)\Windows Kits\8.0\include\shared;C:\Program Files (x86)\Windows Kits\8.0\include\um;C:\Program Files (x86)\Windows Kits\8.0\include\winrt

all: textsearch_senna
.PHONY: all

.SUFFIXES: .c .cpp

.c{$(OBJDIR)}.obj:
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCS) /TC $** /Fo$@
.cpp{$(OBJDIR)}.obj:
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCS) /TP $** /Fo$@

# -- textsearch_senna.c

textsearch_senna: $(BINDIR)\textsearch_senna.dll

$(BINDIR)\textsearch_senna.dll: $(OBJDIR)\textsearch_senna.obj
	@if not exist $(BINDIR) mkdir $(BINDIR)
	$(LINK) $(LIBS) $** /DEF:textsearch_senna.def /OUT:$@

.PHONY: clean
clean:
	rmdir /s /q $(OBJDIR)
	rmdir /s /q $(BINDIR)
