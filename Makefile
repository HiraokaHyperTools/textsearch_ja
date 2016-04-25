
!IFNDEF PGV
!ERROR PGV=9.4 or PGV=9.1
!ENDIF

!IFNDEF CPU
!ERROR CPU=x86 or CPU=x64
!ENDIF

VER=9.0.0

BINDIR = bin\$(PGV)\$(CPU)
OBJDIR = obj\$(PGV)\$(CPU)
CP = copy
!IF "$(CPU)" == "x86"
CXX = "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\cl.exe"
LINK = "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\link.exe"
LIB = C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib;C:\Program Files (x86)\Windows Kits\8.0\lib\win8\um\x86
MECABLIB = libmecab-sdk/libmecab.lib
MACABINC = libmecab-sdk
!ELSEIF "$(CPU)" == "x64"
CXX = "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\x86_amd64\cl.exe"
LINK = "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\x86_amd64\link.exe"
LIB = C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib\amd64;C:\Program Files (x86)\Windows Kits\8.0\lib\win8\um\x64
MECABLIB = libmecab-sdk64/libmecab.lib
MACABINC = libmecab-sdk64
!ELSE
!ERROR CPU Unknown. CPU=x86 or CPU=x64
!ENDIF
CXXFLAGS = /DBUILDING_DLL /DWIN32 /DNDEBUG /D_WINDOWS /D_USRDLL /D_WINDLL /D_ATL_STATIC_REGISTRY /LD /MT /Ot /GL /DUNICODE /c /EHsc 
INCS = /I ../include_$(CPU) /I PGfiles/$(PGV)/include_$(CPU) /I PGfiles/$(PGV)/include_$(CPU)/server /I $(MACABINC)
LIBS = /libpath:"$(LIB)" $(MECABLIB) foo/$(CPU)/foo.lib PGfiles/$(PGV)/lib_$(CPU)/postgres.lib /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /MACHINE:$(CPU) /DLL /RELEASE 
# vsvars32等が定義する環境変数を上書き。特に、x86のLIBが厄介。
INCLUDE = include;C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\INCLUDE;C:\Program Files (x86)\Windows Kits\8.0\include\shared;C:\Program Files (x86)\Windows Kits\8.0\include\um;C:\Program Files (x86)\Windows Kits\8.0\include\winrt

all: textsearch_ja
.PHONY: all

.SUFFIXES: .c .cpp

.c{$(OBJDIR)}.obj:
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCS) /TC $** /Fo$@
.cpp{$(OBJDIR)}.obj:
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCS) /TP $** /Fo$@

# -- textsearch_ja.c

textsearch_ja: $(BINDIR)\textsearch_ja.dll

$(BINDIR)\textsearch_ja.dll: $(OBJDIR)\textsearch_ja.obj $(OBJDIR)\encoding_eucjp.obj $(OBJDIR)\encoding_utf8.obj
	@if not exist $(BINDIR) mkdir $(BINDIR)
	$(LINK) $(LIBS) $** /DEF:textsearch_ja.def /OUT:$@

.PHONY: clean
clean:
	rmdir /s /q $(OBJDIR)
	rmdir /s /q $(BINDIR)

PACKDIR = pack\textsearch_ja-$(VER)-postgresql-$(PGV)-$(CPU)

pack: all \
 $(PACKDIR)\bin \
 $(PACKDIR)\bin\mecabrc \
 $(PACKDIR)\lib \
 $(PACKDIR)\lib\textsearch_ja.dll \
 $(PACKDIR)\share\extension \
 $(PACKDIR)\share\extension\textsearch_ja--9.0.0.sql \
 $(PACKDIR)\share\extension\textsearch_ja.control

$(PACKDIR)\bin:
	mkdir $@
$(PACKDIR)\lib:
	mkdir $@
$(PACKDIR)\share\extension:
	mkdir $@

$(PACKDIR)\bin\mecabrc: mecabrc
	$(CP) $** $@

$(PACKDIR)\lib\textsearch_ja.dll: $(BINDIR)\textsearch_ja.dll
	$(CP) $** $@

$(PACKDIR)\share\extension\textsearch_ja--9.0.0.sql: textsearch_ja-$(PGV).sql.in
	$(CP) $** $@

$(PACKDIR)\share\extension\textsearch_ja.control: textsearch_ja.control
	$(CP) $** $@
