# Microsoft Developer Studio Project File - Name="PrtSys" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PrtSys - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PrtSys.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PrtSys.mak" CFG="PrtSys - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PrtSys - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PrtSys - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PrtSys - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "PrtSys - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "PrtSys - Win32 Release"
# Name "PrtSys - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3dsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\3dsObject.cpp
# End Source File
# Begin Source File

SOURCE=.\3dsReader.cpp
# End Source File
# Begin Source File

SOURCE=.\CamEye.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_AtomicBomb.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_Bmp.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_Fire.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_FireWork.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_Snow.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_WaterFall.cpp
# End Source File
# Begin Source File

SOURCE=.\GLO.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Particle.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtAtomicBomb.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtBMP.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtDemoBit.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtExplode.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtFire.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtFirework.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtSnow.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtSys.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtSys.rc
# End Source File
# Begin Source File

SOURCE=.\PrtSysDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtSysView.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtWaterFall.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TRILIST.CPP
# End Source File
# Begin Source File

SOURCE=.\TRIOBJECT.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3dsDlg.h
# End Source File
# Begin Source File

SOURCE=.\3dsObject.h
# End Source File
# Begin Source File

SOURCE=.\3DSREADER.H
# End Source File
# Begin Source File

SOURCE=.\BASICREAD.H
# End Source File
# Begin Source File

SOURCE=.\CamEye.h
# End Source File
# Begin Source File

SOURCE=.\ColorEdit.h
# End Source File
# Begin Source File

SOURCE=.\DLG_AtomicBomb.h
# End Source File
# Begin Source File

SOURCE=.\DLG_Bmp.h
# End Source File
# Begin Source File

SOURCE=.\DLG_Fire.h
# End Source File
# Begin Source File

SOURCE=.\DLG_FireWork.h
# End Source File
# Begin Source File

SOURCE=.\DLG_Snow.h
# End Source File
# Begin Source File

SOURCE=.\DLG_WaterFall.h
# End Source File
# Begin Source File

SOURCE=.\GLO.h
# End Source File
# Begin Source File

SOURCE=.\GLSTRUCTURES.H
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Particle.h
# End Source File
# Begin Source File

SOURCE=.\PrtSys.h
# End Source File
# Begin Source File

SOURCE=.\PrtSysDoc.h
# End Source File
# Begin Source File

SOURCE=.\PrtSysView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Screen.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TriList.h
# End Source File
# Begin Source File

SOURCE=.\TRIOBJECT.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\fw_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\magnifi.cur
# End Source File
# Begin Source File

SOURCE=.\res\PrtSys.ico
# End Source File
# Begin Source File

SOURCE=.\res\PrtSys.rc2
# End Source File
# Begin Source File

SOURCE=.\res\PrtSysDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
