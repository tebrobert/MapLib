@echo off
set crdr=%cd%
REM C:\Windows\System32\cmd.exe /k C:\WinDDK\7600.16385.1\bin\setenv.bat C:\WinDDK\7600.16385.1\ chk x64 WIN7

REM set ldd="C:\WinDDK\7600.16385.1\bin\setenv.bat C:\WinDDK\7600.16385.1\ chk x64 WIN7"
REM cmd.exe /k %ldd% && cd /d %crdr%
REM cd /d %crdr%

cmd.exe /k C:\WinDDK\7600.16385.1\bin\setenv.bat C:\WinDDK\7600.16385.1\ chk x64 WIN7