@echo off
cls
if exist sources_ec copy sources_ec sources
if exist app.exe del app.exe
build

if exist "objchk_win7_amd64\amd64\app.exe" move "objchk_win7_amd64\amd64\app.exe" .

if exist .binaries               rmdir .binaries         /s /q
if exist .obj                    rmdir .obj              /s /q
if exist objchk_win7_amd64       rmdir objchk_win7_amd64 /s /q
if exist buildchk_win7_amd64.log del buildchk_win7_amd64.log

if exist sources                 del sources
