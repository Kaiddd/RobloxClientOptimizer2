@echo off
cls
title Roblox Client Optimizer2 (RCO2) Un-Installer...

echo We are so sorry to see you leave RCO.
echo Press any key to start the uninstallation.
echo (Note, VCRedist (What makes C++ work) will not be uninstalled, as lots of programs use it, if you'd like to do that, do so yourself, sorry)
echo.
echo ALSO, this will only uninstall RCO the PROGRAM, it will NOT remove your current active roblox fflags, even if they are the RCO fflags, remove them with RCO by disabling RCO first, and then uninstalling it, OR, manually deleting the file yourself in %localappdata%\roblox\versions\*current version here*\ClientSettings

echo.
pause
echo.
taskkill /f /im RCO2.exe
del /q "C:\RClientOptimizer2"
rmdir "C:\RClientOptimizer2"
reg delete "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /v "RCO2" /f

echo.
echo It is okay if you see any errors above, RCO2 should still be uninstalled
echo Done Unintalling.
echo.
pause