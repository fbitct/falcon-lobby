;*******************************************************************************
; This script is compiled with the Nullsoft Scriptable Install System (NSIS)
; to produce a windows installer.
;*******************************************************************************

;*******************************************************************************
; ATTRIBUTES
;*******************************************************************************

Name                 "FalconLobby"
OutFile              "..\FalconLobby-installer.exe"
InstallDir           "$PROGRAMFILES\FalconLobby"
BrandingText         " "

CRCCheck             force
SetCompress          force
SetCompressor        lzma

ShowInstDetails      show
ShowUnInstDetails    show

LicenseData          installer_readme.rtf
LicenseText          Overview Continue
LicenseBkColor       /gray

InstallColors        /windows
XPStyle              on

Page                 license
Page                 directory
Page                 instfiles

UninstPage           uninstConfirm
UninstPage           instfiles

;*******************************************************************************
; INSTALLER
;*******************************************************************************

Section ""
   ReadRegStr        $0 HKCU "Software\FalconLobby" "INSTDIR"
   Exec              '"$0\FalconLobby.exe" -shutdown'
   Sleep             3000

   SetOutPath        $INSTDIR
   File              "Release\FalconLobby.exe"
   File              "help.htm"
   File              /r "help_files"
   File              "settings.ini"
   File              "revision_history.txt"

   SetOutPath        $INSTDIR
   CreateShortcut    "$STARTMENU\Programs\Startup\FalconLobby.lnk" "$INSTDIR\FalconLobby.exe"
   CreateShortcut    "$DESKTOP\FalconLobby.lnk" "$INSTDIR\FalconLobby.exe"
      
   WriteUninstaller  Uninst.exe
   WriteRegStr       HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FalconLobby" "DisplayName" "FalconLobby"
   WriteRegStr       HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FalconLobby" "UninstallString" "$INSTDIR\Uninst.exe"

   WriteRegStr       HKCU "Software\FalconLobby" "INSTDIR" "$INSTDIR"

   Exec              '"$INSTDIR\FalconLobby.exe" -backup'
   Exec              "$INSTDIR\FalconLobby.exe"
SectionEnd

;*******************************************************************************
; UNINSTALLER
;*******************************************************************************

Section "Uninstall"
   Exec              '"$INSTDIR\FalconLobby.exe" -shutdown'
   Sleep             3000
   RMDir             /r $INSTDIR
   Delete            "$STARTMENU\Programs\Startup\FalconLobby.lnk"
   Delete            "$DESKTOP\FalconLobby.lnk"
   DeleteRegKey      HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FalconLobby"
   DeleteRegKey      HKCU "Software\FalconLobby"
SectionEnd
