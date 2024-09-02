# Microsoft Developer Studio Project File - Name="enginev2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=enginev2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "enginev2.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "enginev2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "enginev2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x40b /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "enginev2 - Win32 Release"
# Name "enginev2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\config.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\lista.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\mathematics.cpp
# End Source File
# Begin Source File

SOURCE=.\mmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\path.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\config.hpp
# End Source File
# Begin Source File

SOURCE=.\externs.hpp
# End Source File
# Begin Source File

SOURCE=.\lib\lista.hpp
# End Source File
# Begin Source File

SOURCE=.\mathematics.hpp
# End Source File
# Begin Source File

SOURCE=.\mmgr.h
# End Source File
# Begin Source File

SOURCE=.\nommgr.h
# End Source File
# Begin Source File

SOURCE=.\lib\path.hpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Effect Sources"

# PROP Default_Filter "c;cpp"
# Begin Source File

SOURCE=.\effects\3dripple.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\bubble.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\build.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\crystal.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\databall.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\digital.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\dotfade.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\efekti1.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\efekti2.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\efekti3.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\fadein.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\flubber.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\kasviefekti.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\kasvu.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\keiju.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\kusetus.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\layers.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\lehtipuuefekti.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\lierot.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\metapilar.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\mustikka.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\noise.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\potential.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\refface.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\ripple.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\ripple2.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\ruoho.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\slide.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\start.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\strings.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\tehdas.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\tilpe1.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\tilpe2.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\viimeinen.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\water.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\worm.cpp
# End Source File
# End Group
# Begin Group "Effect Headers"

# PROP Default_Filter "h;hpp"
# Begin Source File

SOURCE=.\effects\3dripple.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\bubble.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\build.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\crystal.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\databall.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\digital.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\dotfade.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\efekti1.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\efekti2.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\efekti3.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\fadein.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\flubber.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\gridtypes.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\kasviefekti.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\kasvu.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\keiju.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\kusetus.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\layers.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\lehtipuuefekti.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\lierot.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\metapilar.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\metatables.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\metatypes.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\mustikka.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\noise.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\potential.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\refface.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\ripple.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\ripple2.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\ruoho.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\slide.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\start.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\strings.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\tehdas.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\tilpe1.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\tilpe2.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\viimeinen.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\water.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\worm.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\dllleaks.log
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\loki.log
# End Source File
# Begin Source File

SOURCE=.\memleaks.log
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\setup.ini
# End Source File
# End Group
# End Target
# End Project
