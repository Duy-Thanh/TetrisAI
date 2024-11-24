@ECHO OFF

SET CURRENT_DIRECTORY=%~dp0
SET VS_PATH=%~1
SET OPTIONS=%~2
SET COMPUTER_NAME=%~3

if "%OPTIONS%" == "/Clean" (
    ECHO Cleaning project
    call "%CURRENT_DIRECTORY%clean.cmd"
) else if "%OPTIONS%" == "/Rebuild" (
    ECHO Rebuilding project
    call "%CURRENT_DIRECTORY%clean.cmd"

    ECHO Visual Studio path: %VS_PATH%

    ECHO Setting up environment

    call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat"

    ECHO Building project in %CURRENT_DIRECTORY%

    cd %CURRENT_DIRECTORY%
    cmake -Bbuild -H.
    cmake --build build --config Release
) else if "%OPTIONS%" == "/Build" (
    ECHO Visual Studio path: %VS_PATH%

    ECHO Setting up environment

    call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat"

    ECHO Building project in %CURRENT_DIRECTORY%

    cd %CURRENT_DIRECTORY%
    cmake -Bbuild -H.
    cmake --build build --config Release
)

pause