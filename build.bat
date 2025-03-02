@echo off

echo Loading settings...
call settings.bat

:: Check if the lib folder contains libraylib.a, libraylibdll.a, and raylib.dll
echo Checking lib folder...
if exist "%raylib_path%\lib\libraylib.a" (
    echo libraylib.a found in lib folder.
) else (
    echo ERROR: libraylib.a not found in lib folder.
	exit /b 1
)

if exist "%raylib_path%\lib\libraylibdll.a" (
    echo libraylibdll.a found in lib folder.
) else (
    echo ERROR: libraylibdll.a not found in lib folder.
	exit /b 1
)

if exist "%raylib_path%\lib\raylib.dll" (
    echo raylib.dll found in lib folder.
) else (
    echo ERROR: raylib.dll not found in lib folder.
	exit /b 1
)

:: Check if the include folder contains raylib.h, raymath.h, and rlgl.h
echo Checking include folder...
if exist "%raylib_path%\include\raylib.h" (
    echo raylib.h found in include folder.
) else (
    echo ERROR: raylib.h not found in include folder.
	exit /b 1
)

if exist "%raylib_path%\include\raymath.h" (
    echo raymath.h found in include folder.
) else (
    echo ERROR: raymath.h not found in include folder.
	exit /b 1
)

if exist "%raylib_path%\include\rlgl.h" (
    echo rlgl.h found in include folder.
) else (
    echo ERROR: rlgl.h not found in include folder.
	exit /b 1
)

:: Finish
echo Raylib successfully installed.

echo Creating build if it doesn't exist...
if not exist build mkdir build	

echo Compiling all .cpp and .c files in src folder...
for %%f in (src\*.cpp src\*.c %custom_src%) do (
    echo Compiling %%~nxf...
    g++ -O3 -I"%raylib_path%\include" -I"src" -I"src/include" %custom_include% -c %%f -o build\%%~nf.o
)

if exist build\raylib.dll (
    del build\raylib.dll   
)

echo Creating executable...
if "%link%"=="dynamic" (
	echo Dynamic linking raylib
	g++ -O3 -mwindows -L"%raylib_path%\lib" -L"src/lib" build\*.o -o build\%name%.exe -l:raylib.dll -lgdi32 -lwinmm
	copy raylib\lib\raylib.dll build\raylib.dll
) else (
	echo Static linking raylib
	g++ -O3 -mwindows -L"%raylib_path%\lib" -L"src/lib" build\*.o -o build\%name%.exe -lraylib -lgdi32 -lwinmm
)

echo Build completed.

echo Cleaning up temporary files...
del build\*.o

echo Cleanup complete.