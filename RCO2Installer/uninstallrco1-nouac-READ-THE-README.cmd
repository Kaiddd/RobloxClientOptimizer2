@echo off
cls
title Roblox Client Optimizer (RCO) Un-Installer...

echo We are so sorry to see you leave RCO.
echo Press any key to start the uninstallation.
echo (Note, VCRedist (What makes C++ work) will not be uninstalled, as lots of programs use it, if you'd like to do that, do so yourself, sorry)
echo.
echo ALSO, this will only uninstall RCO the PROGRAM, it will NOT remove your current active roblox fflags, even if they are the RCO fflags, remove them with RCO by disabling RCO first, and then uninstalling it, OR, manually deleting the file yourself in %localappdata%\roblox\versions\*current version here*\ClientSettings

echo.
pause
echo.
taskkill /f /im RCO.exe
del /q "%appdata%\RClientOptimizer"
rmdir "%appdata%\RClientOptimizer"
del /q "C:\RClientOptimizer"
rmdir "C:\RClientOptimizer"
del /q "C:\Program Files\RCO"
rmdir "C:\Program Files\RCO"
reg delete "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /v "RCO" /f

echo.
echo It is okay (and expected) if you see any errors above, RCO should still be uninstalled
echo Done Unintalling.
echo.
pause