@echo off

set WarningFlags= -WX -W4 -wd4100 -wd4189 -wd4201 -wd4996 -wd4701 -wd4703

if not exist ..\bin mkdir ..\bin
pushd ..\bin
cl -MT -nologo -Gm- -GR- -Oi -EHa- -FC -Zi -Od ..\src\universal_translator.c %WarningFlags%
popd