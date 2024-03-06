@ECHO OFF
REM Run the assembly

ECHO "Running assembly..."

make -f "Makefile.windows.mak" All
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

.\bin\assembly
