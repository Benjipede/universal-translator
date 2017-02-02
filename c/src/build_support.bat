@echo off

set WarningFlags= -WX -W4 -wd4100 -wd4189 -wd4201 -wd4996
set CompilerFlags= -MT -nologo -Gm- -GR- -Oi -EHa- -FC -Zi -Od

pushd ..\misc
cl %CompilerFlags% ..\src\support.c %WarningFlags%
popd