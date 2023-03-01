@echo off
cls
title Roblox Client Optimizer2 (RCO2) Installer...

echo Welcome to RCO Installer!!! ^| (By installing this you agree to all licenses in the licenses folder)
echo Press any key to start the installation!

echo.
pause

echo.
echo Installing VCRedist...
deps\VC_redist.x64.exe /install /quiet /norestart
echo Done! ^| Copying RCO2.exe to C:\RClientOptimizer2
mkdir C:\RClientOptimizer2
copy deps\RCO2.exe C:\RClientOptimizer2
echo Done! ^| Adding RCO2 to startup
reg add "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /v RCO2 /t REG_SZ /d C:\RClientOptimizer\RCO2.exe /f
echo Done! ^| Added RCO2 to startup (Disable in task manager if you would like)
echo.
echo.
echo RCO2 has now been installed!!
echo If you didn't have any version of VCRedist installed beforehand you should restart your PC before running RCO2...
echo Press any key to run RCO2...
echo.
pause
start C:\RClientOptimizer2\RCO2.exe