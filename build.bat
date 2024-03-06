@ECHO OFF
REM Build Everything

ECHO Building everything...

make -f "Makefile.windows.mak" All
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

REM Build Tests

ECHO "All assemblies built successfully."
