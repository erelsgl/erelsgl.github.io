# Microsoft Developer Studio Project File - Name="mchd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=mchd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mchd.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mchd.mak" CFG="mchd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mchd - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mchd - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mchd - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MCHD_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MCHD_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40d /d "NDEBUG"
# ADD RSC /l 0x40d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "mchd - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "c:\temp\debug\"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MCHD_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MCHD_EXPORTS" /YX /FD /GZ  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40d /d "_DEBUG"
# ADD RSC /l 0x40d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mchd - Win32 Release"
# Name "mchd - Win32 Debug"
# Begin Group "Source Files No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\cpp\morph\addlex.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\alghist.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\cpplib\argsfile.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\bw2fw.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\cpplib\cmdline1.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\corpus.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\hpmainit.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\hpmaio.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\hpmamain.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\cpplib\ivrit.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\cpplib\klli1.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\lexicon.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\lexinf.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\mip.cpp
# End Source File
# Begin Source File

SOURCE="..\cpp\morph\mone-nit.cpp"
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\morphanl.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\morphinf.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\morphsik.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\morphst2.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\morphtav.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\morphtyp.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\morphui.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\cpplib\sgt.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\similar.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\sntncinf.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\cpplib\stringt.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\tavnit.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\tiqun3.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\cpplib\ui.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\cpplib\wordcnt.cpp
# End Source File
# End Group
# End Target
# End Project
