#Requires -Version 5.1
# 
# build.ps1 - PowerShell script for building Tetris AI
#
# This file is a part of Tetris AI project and Tetris AI build toolchain project.
#
# Copyright (C) 2024 nekkochan. All rights reserved.
#
# Licensed under the MIT License. See LICENSE file in the project root for full license information.
#

[CmdletBinding()]
param (
    [Parameter(Mandatory = $false)]
    [string]$options = "/Build"
)

# Enable strict mode and error handling
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

# Initialize logging
$logFile = Join-Path $PWD "build_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"
$logDir = Split-Path $logFile -Parent
$transcriptStarted = $false

# Ensure log directory exists
if (-not (Test-Path $logDir)) {
    New-Item -ItemType Directory -Force -Path $logDir | Out-Null
    Write-Host "Created log directory: $logDir"
}

# Start transcript with error handling
try {
    Start-Transcript -Path $logFile -ErrorAction Stop
    $transcriptStarted = $true
    Write-Host "Started logging to: $logFile"
}
catch {
    Write-Warning "Failed to start transcript: $_"
}

try {
    $currentDirectory = $PWD
    
    # Import module with error handling
    $modulePath = Join-Path $currentDirectory "module.ps1"
    if (-not (Test-Path $modulePath)) {
        throw "Module file not found: $modulePath"
    }
    . $modulePath

    # Define directories
    $depsDirectory = Join-Path $currentDirectory "deps"
    $7zDirectory = Join-Path $depsDirectory "7z"
    $fceuxDirectory = Join-Path $depsDirectory "fceux"
    $vswhereDirectory = Join-Path $depsDirectory "vswhere"

    # Function to download file with retry and verification
    function Download-FileWithRetry {
        param (
            [string]$Uri,
            [string]$OutFile,
            [int]$MaxRetries = 3
        )
        
        for ($i = 1; $i -le $MaxRetries; $i++) {
            try {
                Write-Host "Downloading $Uri (Attempt $i of $MaxRetries)..."
                Invoke-WebRequest -Uri $Uri -OutFile $OutFile
                return $true
            }
            catch {
                if ($i -eq $MaxRetries) {
                    throw "Failed to download after $MaxRetries attempts: $_"
                }
                Write-Warning "Download attempt $i failed. Retrying..."
                Start-Sleep -Seconds ($i * 2)
            }
        }
    }

    # Setup vswhere
    Write-Host "Setting up vswhere..."
    New-Item -ItemType Directory -Path $vswhereDirectory -Force | Out-Null
    $vswhereFile = Join-Path $vswhereDirectory "vswhere.exe"
    New-Item -Path (Join-Path $vswhereDirectory ".gitkeep") -ItemType File -Force | Out-Null
    
    if (-not (Test-Path $vswhereFile) -or $true) { # Always update vswhere
        Download-FileWithRetry -Uri "https://github.com/microsoft/vswhere/releases/latest/download/vswhere.exe" -OutFile $vswhereFile
    }

    # Verify vswhere integrity
    Write-Host "Verifying vswhere installation integrity..."
    $signature = Get-AuthenticodeSignature $vswhereFile
    if ($signature.Status -ne 'Valid' -or $signature.SignerCertificate.Subject -notlike '*Microsoft Corporation*') {
        throw "Invalid digital signature for vswhere.exe"
    }

    # Get VS Path
    Write-Host "Getting Visual Studio installation path..."
    $vsPath = & $vswhereFile -latest -property installationPath
    if (-not $vsPath) {
        throw "Failed to locate Visual Studio installation"
    }
    Write-Host "Visual Studio found at: $vsPath"

    Write-Host "Setting up PS2EXE..."
    $moduleBase = Join-Path $env:USERPROFILE "Documents\WindowsPowerShell\Modules\ps2exe"
    Write-Host "Module base path: $moduleBase"

    # Ensure module base directory exists
    if (-not (Test-Path $moduleBase)) {
        Write-Host "Creating module directory: $moduleBase"
        New-Item -ItemType Directory -Force -Path $moduleBase | Out-Null
    }

    # Download and set up PS2EXE files with verbose logging
    $ps2exeFiles = @{
        'ps2exe.psm1' = 'https://raw.githubusercontent.com/MScholtes/PS2EXE/master/Module/ps2exe.psm1'
        'ps2exe.ps1'  = 'https://raw.githubusercontent.com/MScholtes/PS2EXE/master/Module/ps2exe.ps1'
        'ps2exe.psd1' = 'https://raw.githubusercontent.com/MScholtes/PS2EXE/master/Module/ps2exe.psd1'
    }

    foreach ($file in $ps2exeFiles.GetEnumerator()) {
        $filePath = Join-Path $moduleBase $file.Key
        Write-Host "Processing file: $($file.Key) -> $filePath"
        
        try {
            # Remove existing file if it exists
            if (Test-Path $filePath) {
                Write-Host "Removing existing file: $filePath"
                Remove-Item -Path $filePath -Force -ErrorAction Stop
            }

            # Download new file
            Write-Host "Downloading from: $($file.Value)"
            Download-FileWithRetry -Uri $file.Value -OutFile $filePath
            
            # Verify file was created
            if (Test-Path $filePath) {
                Write-Host "Successfully downloaded: $filePath"
            } else {
                throw "File was not created: $filePath"
            }
        }
        catch {
            Write-Error "Error processing file $($file.Key): $_"
            throw
        }
    }

    # Import module with verbose error handling
    $moduleToImport = Join-Path $moduleBase "ps2exe.psm1"
    Write-Host "Attempting to import module from: $moduleToImport"

    if (Test-Path $moduleToImport) {
        try {
            Import-Module $moduleToImport -Force -Verbose
            Write-Host "PS2EXE module imported successfully"
        }
        catch {
            Write-Error "Failed to import module: $_"
            throw
        }
    } else {
        throw "PS2EXE module file not found at: $moduleToImport"
    }

    # Build launcher
    Write-Host "Building launcher..."
    $launcherPath = Join-Path $currentDirectory "launcher.exe"
    $launcherConfigPath = Join-Path $currentDirectory "launcher.exe.config"
    $releaseDir = Join-Path $currentDirectory "build\Release"

    $launcherParams = @{
        x64 = $true
        DPIAware = $true
        MTA = $true
        title = 'Tetris AI Launcher'
        description = 'Tetris AI Launcher'
        company = 'nekkochan'
        product = 'launcher.exe'
        copyright = '(C) 2024 nekkochan. All right reserved'
        trademark = '(C) nekkochan'
        noConsole = $true
        version = '1.0.0.0'
        supportOS = $true
        longPaths = $true
        inputFile = Join-Path $currentDirectory "run.ps1"
        outputFile = $launcherPath
    }
    
    Invoke-ps2exe @launcherParams

    # Copy and cleanup launcher files
    if (Test-Path $launcherPath) {
        New-Item -ItemType Directory -Force -Path $releaseDir | Out-Null
        Copy-Item -Path $launcherPath -Destination $releaseDir -Force
        Remove-Item -Path $launcherPath -Force
    }

    if (Test-Path $launcherConfigPath) {
        Copy-Item -Path $launcherConfigPath -Destination $releaseDir -Force
        Remove-Item -Path $launcherConfigPath -Force
    }

    # Setup dependencies
    Write-Host "Setting up dependencies..."
    
    # Setup 7z
    Write-Host "Setting up 7z..."
    Remove-Item -Path $7zDirectory -Recurse -Force -ErrorAction SilentlyContinue
    New-Item -ItemType Directory -Force -Path $7zDirectory | Out-Null
    New-Item -Path (Join-Path $7zDirectory ".gitkeep") -ItemType File -Force | Out-Null
    
    $7zZipPath = Join-Path $7zDirectory "7z.zip"
    Download-FileWithRetry -Uri "https://duy-thanh.github.io/file/7z.zip" -OutFile $7zZipPath
    Expand-Archive -Path $7zZipPath -DestinationPath $7zDirectory
    Remove-Item -Path $7zZipPath -Force

    # Setup FCEUX
    Write-Host "Setting up FCEUX..."
    Remove-Item -Path $fceuxDirectory -Recurse -Force -ErrorAction SilentlyContinue
    New-Item -ItemType Directory -Force -Path $fceuxDirectory | Out-Null
    New-Item -Path (Join-Path $fceuxDirectory ".gitkeep") -ItemType File -Force | Out-Null

    $fceuxArchivePath = Join-Path $fceuxDirectory "fceux.7z"
    Download-FileWithRetry -Uri "https://duy-thanh.github.io/file/fceux.7z" -OutFile $fceuxArchivePath

    # Extract using 7za.exe
    $7zaPath = Join-Path $7zDirectory "7za.exe"
    if (Test-Path $7zaPath) {
        try {
            Write-Host "Extracting FCEUX using 7za.exe..."
            $extractProcess = Start-Process -FilePath $7zaPath `
                -ArgumentList "x", "`"$fceuxArchivePath`"", "-o`"$fceuxDirectory`"", "-y" `
                -Wait `
                -PassThru `
                -NoNewWindow
            
            if ($extractProcess.ExitCode -ne 0) {
                throw "7za.exe extraction failed with exit code: $($extractProcess.ExitCode)"
            }
            
            # Cleanup archive after successful extraction
            Remove-Item -Path $fceuxArchivePath -Force
            Write-Host "FCEUX extracted successfully"
        }
        catch {
            Write-Error "Failed to extract FCEUX: $_"
            throw
        }
    } else {
        throw "7za.exe not found at: $7zaPath"
    }

    # Start build process
    Write-Host "Starting build process..."
    try {
        $computerNameEncoded = Encode-String $env:COMPUTERNAME
        $buildCmd = Join-Path $currentDirectory "build.cmd"
        
        $buildArgs = "/c call `"$buildCmd`" `"$vsPath`" $options /CN=$computerNameEncoded"
        
        Write-Host "Build command: $buildCmd"
        Write-Host "VS Path: $vsPath"
        Write-Host "Build arguments: $buildArgs"
        Write-Host "Working directory: $currentDirectory"
        
        if (-not (Test-Path $buildCmd)) {
            throw "Build command not found: $buildCmd"
        }
        
        if (-not (Test-Path $vsPath)) {
            throw "Visual Studio path not found: $vsPath"
        }
        
        # Execute build without -Wait
        $buildProcess = Start-Process -FilePath "cmd.exe" `
            -ArgumentList $buildArgs `
            -WorkingDirectory $currentDirectory `
            -PassThru `
            -RedirectStandardOutput "$PWD\build_output.log" `
            -RedirectStandardError "$PWD\build_error.log" `
            -NoNewWindow `
            -ErrorAction Stop

        # Wait with timeout
        $timeout = 300 # 5 minutes timeout
        if (-not $buildProcess.WaitForExit($timeout * 1000)) {
            Write-Warning "Build process taking too long, attempting to terminate..."
            try {
                $buildProcess.Kill()
                $buildProcess.WaitForExit(10000) # Give it 10 seconds to clean up
            } catch {
                Write-Warning "Failed to kill process: $_"
            }
            throw "Build process timed out after $timeout seconds"
        }

        # Check if the build output exists instead of just exit code
        $outputExe = Join-Path $currentDirectory "build\Release\tetrisai.exe"
        if (Test-Path $outputExe) {
            Write-Host "Build completed successfully!"
            Write-Host "Output executable found at: $outputExe"
        } else {
            Write-Host "Build process completed but output not found"
            if (Test-Path "$PWD\build_error.log") {
                Write-Host "Build Error Log:"
                Get-Content "$PWD\build_error.log" | ForEach-Object { Write-Host $_ }
            }
            if (Test-Path "$PWD\build_output.log") {
                Write-Host "Build Output Log:"
                Get-Content "$PWD\build_output.log" | ForEach-Object { Write-Host $_ }
            }
            throw "Build output not found at expected location: $outputExe"
        }
    }
    catch {
        if (Test-Path "$PWD\build\Release\tetrisai.exe") {
            Write-Host "Build appears successful despite error, continuing..."
        } else {
            Write-Error "Build process error: $_"
            if (Test-Path "$PWD\build_error.log") {
                Write-Host "Build Error Log:"
                Get-Content "$PWD\build_error.log" | ForEach-Object { Write-Host $_ }
            }
            throw
        }
    }
}
catch {
    Write-Error "Build failed: $_"
    exit 1
}
finally {
    if ($transcriptStarted) {
        Stop-Transcript
    }
    
    Write-Host "Cleaning up processes..."
    try {
        # Use a job for timeout to prevent hanging
        $job = Start-Job -ScriptBlock { Start-Sleep -Seconds 10 }
        Wait-Job $job -Timeout 11 | Remove-Job -Force
        
        # Kill cmd processes more selectively
        Get-Process cmd -ErrorAction SilentlyContinue | 
            Where-Object { $_.MainWindowTitle -match 'build|cmake|msbuild' } |
            ForEach-Object { 
                try {
                    $_ | Stop-Process -Force -ErrorAction SilentlyContinue
                } catch {
                    Write-Warning "Failed to kill process $($_.Id): $_"
                }
            }
    }
    catch {
        Write-Warning "Cleanup process error: $_"
    }
    
    Write-Host "Press any key to continue..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
}
