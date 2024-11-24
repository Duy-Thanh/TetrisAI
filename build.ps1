# 
# build.ps1 - PowerShell script for building Tetris AI
#
# This file is a part of Tetris AI project and Tetris AI build toolchain project.
#
# Copyright (C) 2024 nekkochan. All rights reserved.
#
# Licensed under the MIT License. See LICENSE file in the project root for full license information.
#

param (
    [Parameter(Mandatory = $false)]
    [string]$options = ""
)

# Default options
if ($options -eq "") {
    $options = "/Build"
}

$currentDirectory = Get-Location

# Import module
. "$currentDirectory\module.ps1"

$depsDirectory = "$currentDirectory\deps"

# Create vswhere directory if it doesn't exist
if (-not (Test-Path "$depsDirectory\vswhere")) {
    New-Item -ItemType Directory -Path "$depsDirectory\vswhere"
}

# Install vswhere
if (-not (Test-Path "$depsDirectory\vswhere\vswhere.exe")) {
    Invoke-WebRequest -Uri "https://github.com/microsoft/vswhere/releases/latest/download/vswhere.exe" -OutFile "$depsDirectory\vswhere\vswhere.exe"
} else {
    Write-Host "vswhere.exe version is old. Updating..."
    Remove-Item "$depsDirectory\vswhere\vswhere.exe" -Force
    Invoke-WebRequest -Uri "https://github.com/microsoft/vswhere/releases/latest/download/vswhere.exe" -OutFile "$depsDirectory\vswhere\vswhere.exe"
}

# Setup vswhere path
$vswhereFile = "$depsDirectory\vswhere\vswhere.exe"

Write-Host "Verifying vswhere installation integrity"

# Verify vswhere integrity
$signature = Get-AuthenticodeSignature $vswhereFile
if ($signature.Status -ne 'Valid' -or $signature.SignerCertificate.Subject -notlike '*Microsoft Corporation*') {
    Remove-Item $vswhereFile -Force
    Write-Host "Error: Invalid digital signature for vswhere.exe" -ForegroundColor Red
    exit 1
}

Write-Host "Getting Visual Studio installation path"

$vsPath = $null

try {
    $vsPath = & $vswhereFile -latest -property installationPath
} catch {
    Write-Host "Error: Failed to get Visual Studio path" -ForegroundColor Red
    exit 1
}

Write-Host "Visual Studio installation path: $vsPath"

# Build target for each computer based on computer name.....
$computerName = $env:COMPUTERNAME

Write-Host "Computer name: $computerName"

$computerNameEncoded = Encode-String $computerName

# Building scripts
Write-Host "Building launcher..."

# Use ps2exe without installation
$moduleBase = "$env:USERPROFILE\Documents\WindowsPowerShell\Modules\ps2exe"
$ps2exeModule = "$moduleBase\ps2exe.psm1"
$ps2exeScript = "$moduleBase\ps2exe.ps1"
$ps2exePSD = "$moduleBase\ps2exe.psd1"

Remove-Item $ps2exeModule -Force

# Create module directory
New-Item -ItemType Directory -Force -Path $moduleBase

# Download module files
$files = @{
    'ps2exe.psm1' = 'https://raw.githubusercontent.com/MScholtes/PS2EXE/master/Module/ps2exe.psm1'
    'ps2exe.ps1' = 'https://raw.githubusercontent.com/MScholtes/PS2EXE/master/Module/ps2exe.ps1'
    'ps2exe.psd1' = 'https://raw.githubusercontent.com/MScholtes/PS2EXE/master/Module/ps2exe.psd1'
}

foreach ($file in $files.Keys) {
    $dest = Join-Path $moduleBase $file
    Write-Host "Downloading $file..."
    Invoke-WebRequest -Uri $files[$file] -OutFile $dest
}

Import-Module $ps2exeModule -Force

Invoke-ps2exe -x64 -DPIAware -MTA -title 'Tetris AI Launcher' -description 'Tetris AI Launcher' -company 'nekkochan' -product 'launcher.exe' -copyright '(C) 2024 nekkochan. All right reserved' -trademark '(C) nekkochan' -noConsole -version '1.0.0.0' -supportOS -longPaths -inputFile "$currentDirectory\run.ps1" -outputFile "$currentDirectory\launcher.exe"

Write-Host "Building launcher successfully. Output file: $currentDirectory\launcher.exe..."
Write-Host "Copying launcher to $currentDirectory\build\bin\Release..."

Copy-Item "$currentDirectory\launcher.exe" -Destination "$currentDirectory\build\Release" -Force
Copy-Item "$currentDirectory\launcher.exe.config" -Destination "$currentDirectory\build\Release" -Force

Write-Host "Cleaning up..."
Remove-Item "$currentDirectory\launcher.exe" -Force
Remove-Item "$currentDirectory\launcher.exe.config" -Force

# Start build process
Start-Process -FilePath "cmd.exe" -ArgumentList "/c $currentDirectory\build.cmd `"$vsPath`" $options /CN=$computerNameEncoded" -WorkingDirectory $currentDirectory -Wait

# End of Scripts
Write-Host "Done!"
Write-Host "Press any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")