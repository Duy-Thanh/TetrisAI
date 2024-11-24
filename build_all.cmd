@ECHO OFF
SET CURRENT_DIRECTORY=%~dp0

ECHO Building all projects...

del /f /s /q "%CURRENT_DIRECTORY%\build.exe"
del /f /s /q "%CURRENT_DIRECTORY%\*.log"
del /f /s /q "%CURRENT_DIRECTORY%\*.exe.config"

REM Use Windows PowerShell 5.1 specifically and skip module loading/installation
"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -Command "if (-not (Test-Path \"$env:USERPROFILE\Documents\WindowsPowerShell\Modules\ps2exe\")) { $url = 'https://raw.githubusercontent.com/MScholtes/TechNet-Gallery/master/PS2EXE-GUI/ps2exe.psm1'; $dest = \"$env:USERPROFILE\Documents\WindowsPowerShell\Modules\ps2exe\ps2exe.psm1\"; New-Item -ItemType Directory -Force -Path (Split-Path $dest); Invoke-WebRequest -Uri $url -OutFile $dest; }; Import-Module ps2exe -Force; Invoke-ps2exe -x64 -DPIAware -MTA -title 'Tetris AI build toolchain' -description 'Tetris AI build toolchain' -company 'CyberDay Studios' -product 'build.exe' -copyright '(C) 2024 CyberDay Studios. All right reserved' -trademark '(C) CyberDay Studios' -version '1.0.0.0' -supportOS -longPaths -inputFile '%CURRENT_DIRECTORY%\build.ps1' -outputFile '%CURRENT_DIRECTORY%\build.exe'"

IF EXIST "%CURRENT_DIRECTORY%\build.exe" (
    ECHO Build.exe created successfully.
    call "%CURRENT_DIRECTORY%\build.exe"
)

ECHO DONE!