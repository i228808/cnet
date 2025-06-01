@echo off
echo Cleaning previous build files...
rmdir /s /q out
del *.o *.exe *.dll *.lib *.exp *.vec *.sca *.vci *.csv

echo Regenerating makefiles...
opp_makemake -f --deep

echo Building project...
nmake -f Makefile.vc
if %errorlevel% neq 0 goto error

echo Build successful!
echo Run the simulation with:
echo .\out\clang-release\src\CNETFINAL.exe -f simulations\omnetpp.ini
goto end

:error
echo Build failed!

:end 