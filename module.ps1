# 
# module.ps1 - Module cryptography for PowerShell build toolchain
#
# This file is a part of ExperienceEngine project and ExperienceEngine build toolchain project.
#
# Copyright (C) 2024 nekkochan. All rights reserved.
#
# Licensed under the MIT License. See LICENSE file in the project root for full license information.
#

function Encode-String($string) {
    $byteString = [System.Text.Encoding]::Unicode.GetBytes($string)
    $encodedString = [System.Convert]::ToBase64String($byteString)
    return $encodedString
}

function Decode-String($encodedString) {
    $byteString = [System.Convert]::FromBase64String($encodedString)
    $decodedString = [System.Text.Encoding]::Unicode.GetString($byteString)
    return $decodedString
}