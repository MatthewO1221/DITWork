@echo off

cd %~dp0

rd /S /Q .\.vs
rd /S /Q .\Debug
rd /S /Q .\Release
rd /S /Q .\x64
rd /S /Q .\x86
del .\*.vcxproj.user

exit /B