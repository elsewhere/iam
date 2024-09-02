# Microsoft Developer Studio Generated NMAKE File, Based on enginev2.dsp
!IF "$(CFG)" == ""
CFG=enginev2 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to enginev2 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "enginev2 - Win32 Release" && "$(CFG)" != "enginev2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "enginev2.mak" CFG="enginev2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "enginev2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "enginev2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "enginev2 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\enginev2.exe"


CLEAN :
	-@erase "$(INTDIR)\3dripple.obj"
	-@erase "$(INTDIR)\bubble.obj"
	-@erase "$(INTDIR)\build.obj"
	-@erase "$(INTDIR)\config.obj"
	-@erase "$(INTDIR)\crystal.obj"
	-@erase "$(INTDIR)\databall.obj"
	-@erase "$(INTDIR)\digital.obj"
	-@erase "$(INTDIR)\dotfade.obj"
	-@erase "$(INTDIR)\efekti1.obj"
	-@erase "$(INTDIR)\efekti2.obj"
	-@erase "$(INTDIR)\efekti3.obj"
	-@erase "$(INTDIR)\fadein.obj"
	-@erase "$(INTDIR)\flubber.obj"
	-@erase "$(INTDIR)\kasviefekti.obj"
	-@erase "$(INTDIR)\kasvu.obj"
	-@erase "$(INTDIR)\keiju.obj"
	-@erase "$(INTDIR)\layers.obj"
	-@erase "$(INTDIR)\lehtipuuefekti.obj"
	-@erase "$(INTDIR)\lierot.obj"
	-@erase "$(INTDIR)\lista.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\mathematics.obj"
	-@erase "$(INTDIR)\metapilar.obj"
	-@erase "$(INTDIR)\mmgr.obj"
	-@erase "$(INTDIR)\mustikka.obj"
	-@erase "$(INTDIR)\noise.obj"
	-@erase "$(INTDIR)\path.obj"
	-@erase "$(INTDIR)\potential.obj"
	-@erase "$(INTDIR)\refface.obj"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\ripple.obj"
	-@erase "$(INTDIR)\ripple2.obj"
	-@erase "$(INTDIR)\ruoho.obj"
	-@erase "$(INTDIR)\slide.obj"
	-@erase "$(INTDIR)\strings.obj"
	-@erase "$(INTDIR)\tehdas.obj"
	-@erase "$(INTDIR)\tilpe1.obj"
	-@erase "$(INTDIR)\tilpe2.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\viimeinen.obj"
	-@erase "$(INTDIR)\water.obj"
	-@erase "$(INTDIR)\worm.obj"
	-@erase "$(OUTDIR)\enginev2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\enginev2.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x40b /fo"$(INTDIR)\resource.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\enginev2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\enginev2.pdb" /machine:I386 /out:"$(OUTDIR)\enginev2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\config.obj" \
	"$(INTDIR)\lista.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mathematics.obj" \
	"$(INTDIR)\mmgr.obj" \
	"$(INTDIR)\path.obj" \
	"$(INTDIR)\3dripple.obj" \
	"$(INTDIR)\bubble.obj" \
	"$(INTDIR)\build.obj" \
	"$(INTDIR)\crystal.obj" \
	"$(INTDIR)\databall.obj" \
	"$(INTDIR)\digital.obj" \
	"$(INTDIR)\dotfade.obj" \
	"$(INTDIR)\efekti1.obj" \
	"$(INTDIR)\efekti2.obj" \
	"$(INTDIR)\efekti3.obj" \
	"$(INTDIR)\fadein.obj" \
	"$(INTDIR)\flubber.obj" \
	"$(INTDIR)\kasviefekti.obj" \
	"$(INTDIR)\kasvu.obj" \
	"$(INTDIR)\keiju.obj" \
	"$(INTDIR)\layers.obj" \
	"$(INTDIR)\lehtipuuefekti.obj" \
	"$(INTDIR)\lierot.obj" \
	"$(INTDIR)\metapilar.obj" \
	"$(INTDIR)\mustikka.obj" \
	"$(INTDIR)\noise.obj" \
	"$(INTDIR)\potential.obj" \
	"$(INTDIR)\refface.obj" \
	"$(INTDIR)\ripple.obj" \
	"$(INTDIR)\ripple2.obj" \
	"$(INTDIR)\ruoho.obj" \
	"$(INTDIR)\slide.obj" \
	"$(INTDIR)\strings.obj" \
	"$(INTDIR)\tehdas.obj" \
	"$(INTDIR)\tilpe1.obj" \
	"$(INTDIR)\tilpe2.obj" \
	"$(INTDIR)\water.obj" \
	"$(INTDIR)\worm.obj" \
	"$(INTDIR)\resource.res" \
	"$(INTDIR)\viimeinen.obj"

"$(OUTDIR)\enginev2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "enginev2 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\enginev2.exe"


CLEAN :
	-@erase "$(INTDIR)\3dripple.obj"
	-@erase "$(INTDIR)\bubble.obj"
	-@erase "$(INTDIR)\build.obj"
	-@erase "$(INTDIR)\config.obj"
	-@erase "$(INTDIR)\crystal.obj"
	-@erase "$(INTDIR)\databall.obj"
	-@erase "$(INTDIR)\digital.obj"
	-@erase "$(INTDIR)\dotfade.obj"
	-@erase "$(INTDIR)\efekti1.obj"
	-@erase "$(INTDIR)\efekti2.obj"
	-@erase "$(INTDIR)\efekti3.obj"
	-@erase "$(INTDIR)\fadein.obj"
	-@erase "$(INTDIR)\flubber.obj"
	-@erase "$(INTDIR)\kasviefekti.obj"
	-@erase "$(INTDIR)\kasvu.obj"
	-@erase "$(INTDIR)\keiju.obj"
	-@erase "$(INTDIR)\layers.obj"
	-@erase "$(INTDIR)\lehtipuuefekti.obj"
	-@erase "$(INTDIR)\lierot.obj"
	-@erase "$(INTDIR)\lista.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\mathematics.obj"
	-@erase "$(INTDIR)\metapilar.obj"
	-@erase "$(INTDIR)\mmgr.obj"
	-@erase "$(INTDIR)\mustikka.obj"
	-@erase "$(INTDIR)\noise.obj"
	-@erase "$(INTDIR)\path.obj"
	-@erase "$(INTDIR)\potential.obj"
	-@erase "$(INTDIR)\refface.obj"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\ripple.obj"
	-@erase "$(INTDIR)\ripple2.obj"
	-@erase "$(INTDIR)\ruoho.obj"
	-@erase "$(INTDIR)\slide.obj"
	-@erase "$(INTDIR)\strings.obj"
	-@erase "$(INTDIR)\tehdas.obj"
	-@erase "$(INTDIR)\tilpe1.obj"
	-@erase "$(INTDIR)\tilpe2.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\viimeinen.obj"
	-@erase "$(INTDIR)\water.obj"
	-@erase "$(INTDIR)\worm.obj"
	-@erase "$(OUTDIR)\enginev2.exe"
	-@erase "$(OUTDIR)\enginev2.ilk"
	-@erase "$(OUTDIR)\enginev2.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\enginev2.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x40b /fo"$(INTDIR)\resource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\enginev2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\enginev2.pdb" /debug /machine:I386 /out:"$(OUTDIR)\enginev2.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\config.obj" \
	"$(INTDIR)\lista.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mathematics.obj" \
	"$(INTDIR)\mmgr.obj" \
	"$(INTDIR)\path.obj" \
	"$(INTDIR)\3dripple.obj" \
	"$(INTDIR)\bubble.obj" \
	"$(INTDIR)\build.obj" \
	"$(INTDIR)\crystal.obj" \
	"$(INTDIR)\databall.obj" \
	"$(INTDIR)\digital.obj" \
	"$(INTDIR)\dotfade.obj" \
	"$(INTDIR)\efekti1.obj" \
	"$(INTDIR)\efekti2.obj" \
	"$(INTDIR)\efekti3.obj" \
	"$(INTDIR)\fadein.obj" \
	"$(INTDIR)\flubber.obj" \
	"$(INTDIR)\kasviefekti.obj" \
	"$(INTDIR)\kasvu.obj" \
	"$(INTDIR)\keiju.obj" \
	"$(INTDIR)\layers.obj" \
	"$(INTDIR)\lehtipuuefekti.obj" \
	"$(INTDIR)\lierot.obj" \
	"$(INTDIR)\metapilar.obj" \
	"$(INTDIR)\mustikka.obj" \
	"$(INTDIR)\noise.obj" \
	"$(INTDIR)\potential.obj" \
	"$(INTDIR)\refface.obj" \
	"$(INTDIR)\ripple.obj" \
	"$(INTDIR)\ripple2.obj" \
	"$(INTDIR)\ruoho.obj" \
	"$(INTDIR)\slide.obj" \
	"$(INTDIR)\strings.obj" \
	"$(INTDIR)\tehdas.obj" \
	"$(INTDIR)\tilpe1.obj" \
	"$(INTDIR)\tilpe2.obj" \
	"$(INTDIR)\water.obj" \
	"$(INTDIR)\worm.obj" \
	"$(INTDIR)\resource.res" \
	"$(INTDIR)\viimeinen.obj"

"$(OUTDIR)\enginev2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("enginev2.dep")
!INCLUDE "enginev2.dep"
!ELSE 
!MESSAGE Warning: cannot find "enginev2.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "enginev2 - Win32 Release" || "$(CFG)" == "enginev2 - Win32 Debug"
SOURCE=.\config.cpp

"$(INTDIR)\config.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\lib\lista.cpp

"$(INTDIR)\lista.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mathematics.cpp

"$(INTDIR)\mathematics.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mmgr.cpp

"$(INTDIR)\mmgr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\lib\path.cpp

"$(INTDIR)\path.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\3dripple.cpp

"$(INTDIR)\3dripple.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\bubble.cpp

"$(INTDIR)\bubble.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\build.cpp

"$(INTDIR)\build.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\crystal.cpp

"$(INTDIR)\crystal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\databall.cpp

"$(INTDIR)\databall.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\digital.cpp

"$(INTDIR)\digital.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\dotfade.cpp

"$(INTDIR)\dotfade.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\efekti1.cpp

"$(INTDIR)\efekti1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\efekti2.cpp

"$(INTDIR)\efekti2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\efekti3.cpp

"$(INTDIR)\efekti3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\fadein.cpp

"$(INTDIR)\fadein.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\flubber.cpp

"$(INTDIR)\flubber.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\kasviefekti.cpp

"$(INTDIR)\kasviefekti.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\kasvu.cpp

"$(INTDIR)\kasvu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\keiju.cpp

"$(INTDIR)\keiju.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\layers.cpp

"$(INTDIR)\layers.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\lehtipuuefekti.cpp

"$(INTDIR)\lehtipuuefekti.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\lierot.cpp

"$(INTDIR)\lierot.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\metapilar.cpp

"$(INTDIR)\metapilar.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\mustikka.cpp

"$(INTDIR)\mustikka.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\noise.cpp

"$(INTDIR)\noise.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\potential.cpp

"$(INTDIR)\potential.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\refface.cpp

"$(INTDIR)\refface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\ripple.cpp

"$(INTDIR)\ripple.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\ripple2.cpp

"$(INTDIR)\ripple2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\ruoho.cpp

"$(INTDIR)\ruoho.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\slide.cpp

"$(INTDIR)\slide.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\strings.cpp

"$(INTDIR)\strings.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\tehdas.cpp

"$(INTDIR)\tehdas.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\tilpe1.cpp

"$(INTDIR)\tilpe1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\tilpe2.cpp

"$(INTDIR)\tilpe2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\viimeinen.cpp

"$(INTDIR)\viimeinen.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\water.cpp

"$(INTDIR)\water.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\effects\worm.cpp

"$(INTDIR)\worm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\resource.rc

"$(INTDIR)\resource.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

