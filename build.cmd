@echo off

if "%1" == "x64" (
    echo platform is %1
) else if "%1" == "Win32" (
    echo platform is %1
) else (
    echo not support platform %1
    exit /b
)

SET PLATFORM=%1
SET PATH=%cd%\tools;%cd%\tools\cmake\bin;%PATH%
SET ROOT_PATH=%cd%
SET BUILD_SYSTEM="Visual Studio 17 2022"
SET SDK_VERSION=10.0.22000.0
SET TOOLSET=v143
SET BUILD_PATH_NAME=build-%PLATFORM%

cd %ROOT_PATH%
call rmdir /s /q %BUILD_PATH_NAME%
call mkdir %BUILD_PATH_NAME%
call cmake -S . -B %BUILD_PATH_NAME% -A %PLATFORM% -G %BUILD_SYSTEM% -T %TOOLSET% -DCMAKE_SYSTEM_VERSION=%SDK_VERSION%
cd %BUILD_PATH_NAME%
call cmake --build . --config Debug
call cmake --build . --config RelWithDebInfo

cd %ROOT_PATH%
call rmdir /s /q %BUILD_PATH_NAME%-StaticRuntime
call mkdir %BUILD_PATH_NAME%-StaticRuntime
call cmake -S . -B %BUILD_PATH_NAME%-StaticRuntime -A %PLATFORM% -G %BUILD_SYSTEM% -T %TOOLSET% -DCMAKE_SYSTEM_VERSION=%SDK_VERSION% -DMSVC_STATIC_RUNTIME=ON -DBUILD_SHARED_LIBS=OFF
cd %BUILD_PATH_NAME%-StaticRuntime
call cmake --build . --config Debug
call cmake --build . --config RelWithDebInfo
