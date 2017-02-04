@echo off

set WarningFlags= -WX -W4 -wd4100 -wd4189 -wd4201 -wd4996 -wd4090
set CompilerFlags= -MT -nologo -Gm- -GR- -Oi -EHa- -FC -Zi -Od

if not exist ..\bin mkdir ..\bin
pushd ..\bin
cl %CompilerFlags% ..\src\interfaces\unitrl.c %WarningFlags%
popd