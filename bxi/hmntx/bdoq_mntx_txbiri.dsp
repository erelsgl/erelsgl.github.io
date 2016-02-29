# Microsoft Developer Studio Project File - Name="bdoq_mntx_txbiri" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=bdoq_mntx_txbiri - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bdoq_mntx_txbiri.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bdoq_mntx_txbiri.mak" CFG="bdoq_mntx_txbiri - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "bdoq_mntx_txbiri - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "bdoq_mntx_txbiri - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "bdoq_mntx_txbiri - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x40d /d "NDEBUG"
# ADD RSC /l 0x40d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "bdoq_mntx_txbiri - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x40d /d "_DEBUG"
# ADD RSC /l 0x40d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "bdoq_mntx_txbiri - Win32 Release"
# Name "bdoq_mntx_txbiri - Win32 Debug"
# Begin Group "Source Files"

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

SOURCE=..\cpp\morph\cimcumim.cpp
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

SOURCE=..\cpp\morph\sntncas.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\sntncinf.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\soi.cpp
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
# Begin Source File

SOURCE=.\bdoq_mntx_txbiri.txt
# End Source File
# Begin Source File

SOURCE=..\cpp\morph\BdoqMntxTxbiri.main.cpp
# End Source File
# End Target
# End Project
