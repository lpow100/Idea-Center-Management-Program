@echo off
powershell -Command ^
  "$j1 = Start-Job { Set-Location '%~dp0frontend'; .\run.bat }; ^
   $j2 = Start-Job { Set-Location '%~dp0backend'; .\main.exe }; ^
   try { Wait-Job $j1, $j2 } ^
   finally { Stop-Job $j1, $j2; Remove-Job $j1, $j2; Write-Host 'Stopped everything.' }"
pause