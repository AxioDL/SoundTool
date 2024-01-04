# Microsoft Developer Studio Project File - Name="mud_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=mud_dll - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mud.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mud.mak" CFG="mud_dll - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mud_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mud_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mud_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MUD_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /I "C:\Program Files\Microsoft SDK\include" /I "./include" /I "../f5/include" /I "../mpimanager/include" /I "../musyx_io/include" /I "../diskio/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MUD_EXPORTS" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib rpcrt4.lib mpr.lib scriptHOSTDLL.lib musyx_io.lib /nologo /dll /machine:I386 /out:"../mud.dll" /libpath:"C:\Program Files\Microsoft SDK\lib" /libpath:"../lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "mud_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MUD_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\Microsoft SDK\include" /I "./include" /I "../f5/include" /I "../mpimanager/include" /I "../musyx_io/include" /I "../diskio/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MUD_EXPORTS" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib rpcrt4.lib shlwapi.lib mpr.lib scriptHOSTDLL.lib musyx_io_d.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"libcd" /out:"../mud_d.dll" /pdbtype:sept /libpath:"C:\Program Files\Microsoft SDK\lib" /libpath:"../lib"

!ENDIF 

# Begin Target

# Name "mud_dll - Win32 Release"
# Name "mud_dll - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\DLLMain.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MUDBaseInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\musyx_io\src\MUDBaseNode.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MUDBaseNodeSet.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MUDDataBaseBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MUDMPIManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MUDServiceInOut.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MUDServiceInOutInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MUDUndoList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "mud"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\mud\Globals.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDAccessBaseInterface.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDBaseInterface.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDBaseNodeInterface.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDBaseNodeInterfaceList.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDBaseNodeSet.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDDataBaseBuilder.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDDate.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDDeletedNode.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDIDTransInfo.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDIDTransList.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDModificationInfo.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDMPIManager.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDNodeCallbackList.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDProperties.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDRootInterface.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDServiceDataBase.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDServiceDataBaseInterface.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDServiceInOut.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDServiceInOutInterface.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDServiceScript.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDServiceScriptInterface.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDTypes.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDTypeStorage.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDUndoEntry.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDUndoGroup.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDUndoInfo.h
# End Source File
# Begin Source File

SOURCE=.\include\mud\MUDUndoList.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\scripthost.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
