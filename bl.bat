@echo off
cls
if exist sources_l copy sources_l sources
if exist lib.lib del lib.lib
build

if exist "objchk_win7_amd64\amd64\lib.lib" move "objchk_win7_amd64\amd64\lib.lib" .

if exist .binaries               rmdir .binaries         /s /q
if exist .obj                    rmdir .obj              /s /q
if exist objchk_win7_amd64       rmdir objchk_win7_amd64 /s /q
if exist buildchk_win7_amd64.log del buildchk_win7_amd64.log

if exist sources                 del sources
