# Microsoft Developer Studio Project File - Name="libiax2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libiax2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libiax2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libiax2.mak" CFG="libiax2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libiax2 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libiax2 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libiax2"
# PROP Scc_LocalPath "..\..\..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libiax2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\libiax2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "LIBIAX" /D "_CRT_SECURE_NO_DEPRECATE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libiax2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug\libiax2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "LIBIAX" /D "_CRT_SECURE_NO_DEPRECATE" /D NTDDI_VERSION=NTDDI_WIN2KSP4 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libiax2 - Win32 Release"
# Name "libiax2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\iax.c
# End Source File
# Begin Source File

SOURCE="..\..\..\lib\libiax2\src\iax2-parser.c"
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\jitterbuf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\md5.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\answer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\busy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\dialtone.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\frame.h
# End Source File
# Begin Source File

SOURCE="..\..\..\lib\libiax2\src\iax-client.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\iax.h
# End Source File
# Begin Source File

SOURCE="..\..\..\lib\libiax2\src\iax2-parser.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\iax2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\jitterbuf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\md5.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\miniphone.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\options.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\ring10.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libiax2\src\ringtone.h
# End Source File
# End Group
# End Target
# End Project
