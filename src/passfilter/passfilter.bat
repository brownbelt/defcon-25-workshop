set filepath=%~dp0

copy "%filepath%x64\Release\passfilter.dll" "%WINDIR%\system32\passfilter.dll" /y

reg.exe add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v "Notification Packages" /d "scecli"\0"passfilter"\0 /t REG_MULTI_SZ /f

pause