@echo off
rem universal-modder-cpp launcher for cmd and PowerShell: runs build\um.exe (a release binary or `make windows`).
if not exist "%~dp0..\build\um.exe" (
  echo um: build\um.exe is missing. Download it from https://github.com/esoexe/universal-modder-cpp/releases 1>&2
  echo     and put it in "%~dp0..\build\", or build it with `make windows` ^(MinGW-w64^). 1>&2
  exit /b 1
)
"%~dp0..\build\um.exe" %*
