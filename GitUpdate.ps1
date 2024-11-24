param(
    [string]$commitMessage
)

Start-Process -FilePath "cmd.exe" -ArgumentList "/c", "git_update.cmd", "`"$commitMessage`"" -WorkingDirectory $PWD -Wait