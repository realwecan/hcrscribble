@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by HCRSCRIBBLE.HPJ. >"hlp\HCRScribble.hm"
echo. >>"hlp\HCRScribble.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\HCRScribble.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\HCRScribble.hm"
echo. >>"hlp\HCRScribble.hm"
echo // Prompts (IDP_*) >>"hlp\HCRScribble.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\HCRScribble.hm"
echo. >>"hlp\HCRScribble.hm"
echo // Resources (IDR_*) >>"hlp\HCRScribble.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\HCRScribble.hm"
echo. >>"hlp\HCRScribble.hm"
echo // Dialogs (IDD_*) >>"hlp\HCRScribble.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\HCRScribble.hm"
echo. >>"hlp\HCRScribble.hm"
echo // Frame Controls (IDW_*) >>"hlp\HCRScribble.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\HCRScribble.hm"
REM -- Make help for Project HCRSCRIBBLE


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\HCRScribble.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\HCRScribble.hlp" goto :Error
if not exist "hlp\HCRScribble.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\HCRScribble.hlp" Debug
if exist Debug\nul copy "hlp\HCRScribble.cnt" Debug
if exist Release\nul copy "hlp\HCRScribble.hlp" Release
if exist Release\nul copy "hlp\HCRScribble.cnt" Release
echo.
goto :done

:Error
echo hlp\HCRScribble.hpj(1) : error: Problem encountered creating help file

:done
echo.
