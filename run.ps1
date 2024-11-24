# Function to generate a random hex
function Get-RandomHex {
    $bytes = New-Object byte[] 8
    $rng = [System.Security.Cryptography.RNGCryptoServiceProvider]::new()
    $rng.GetBytes($bytes)
    $bigInt = [System.Numerics.BigInteger]::new($bytes)
    if ($bigInt -lt 0) {
        $bigInt = -$bigInt
    }
    return "0x" + ($bigInt.ToString("X16"))
}

# Append the random hexadecimal to the ArgumentList
$arguments = @(
    # "/runningMode", "WIN_RELEASE",
    # "/logPath", "$PWD\logs",
    # "/logLevel", "DEFAULT",
    # "/logLevelNumber", "1",
    # "/logFile", "ExperienceEngine.log",
    # "/logConsole", "true",
    # "/data", (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex), ",",
    # (Get-RandomHex), ",", (Get-RandomHex)
)

Start-Process -FilePath "tetrisai.exe" `
    -ArgumentList $arguments `
    -WorkingDirectory $PWD `
    -Wait