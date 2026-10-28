@echo off
set filename=sieve

cl /nologo /DPROF /Oi /GR- /Gm- /GL /EHa- /WX /wd4996 /wd4701 /wd4703 /wd4101 /wd4700 /wd4189 /wd4459 /wd4067 /wd4100 /wd4456 /wd4715 /wd4530 /W3 /FC /MT /O2 /Fo"%CD%\executables\%filename%" /Fe"%CD%\executables\%filename%" "src\sieve.cpp" /link user32.lib onecore.lib
::del "%CD%\executables\%filename%.obj"

echo executing...
"%CD%\executables\%filename%"

::TODO: check if __restrict with pointers work or not.