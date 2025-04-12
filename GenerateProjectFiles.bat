@echo off
call setup\premake\premake5.exe vs2022
cd ExperimentEngine\Dependencies\Reflector
call GenerateProjectFiles.bat
pause