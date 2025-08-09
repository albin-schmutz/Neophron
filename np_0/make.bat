REM Needs Microsoft C Compiler cl.exe

IF "%~1"=="clean" GOTO clean

cl system_test.c system.c -DWIN32
system_test.exe

GOTO end

:clean

DEL *.obj
DEL *.exe
DEL tmp

:end
