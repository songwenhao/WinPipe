@echo off

if "%1" == "x64" (
    echo platform is %1
) else if "%1" == "Win32" (
    echo platform is %1
) else (
    echo not support platform %1
    exit /b
)

SET PATH=%cd%\tools;%cd%\tools\cmake\bin;%PATH%

SET ROOT_PATH=%cd%

SET PLATFORM=%1
SET BUILD_SYSTEM="Visual Studio 17 2022"
SET SDK_VERSION=10.0.22000.0
SET TOOLSET=v143

::protobuf
cd %ROOT_PATH%\protobuf
call git checkout v3.21.12
call rmdir /s /q build
call mkdir build
call cmake -S . -B build -A %PLATFORM% -G %BUILD_SYSTEM% -T %TOOLSET% -DCMAKE_SYSTEM_VERSION=%SDK_VERSION% -DBUILD_SHARED_LIBS=ON -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB_DEFAULT=OFF -DCMAKE_INSTALL_PREFIX:PATH=install
cd build
call cmake --build . --target INSTALL --config Debug
call cmake --build . --target INSTALL --config RelWithDebInfo
call rmdir /s /q ..\libs\%PLATFORM%
call xcopy install\include ..\libs\includes\ /s /e /y
call xcopy Debug\*.dll ..\libs\%PLATFORM%\ /y
call xcopy Debug\*.lib ..\libs\%PLATFORM%\ /y
call xcopy Debug\*.pdb ..\libs\%PLATFORM%\ /y
call xcopy Debug\*.exe ..\libs\%PLATFORM%\ /y
call xcopy RelWithDebInfo\*.dll ..\libs\%PLATFORM%\ /y
call xcopy RelWithDebInfo\*.lib ..\libs\%PLATFORM%\ /y
call xcopy RelWithDebInfo\*.pdb ..\libs\%PLATFORM%\ /y
call xcopy RelWithDebInfo\*.exe ..\libs\%PLATFORM%\ /y

cd ..
call rmdir /s /q build-StaticRuntime
call mkdir build-StaticRuntime
call cmake -S . -B build-StaticRuntime -A %PLATFORM% -G %BUILD_SYSTEM% -T %TOOLSET% -DCMAKE_SYSTEM_VERSION=%SDK_VERSION% -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB_DEFAULT=OFF -Dprotobuf_MSVC_STATIC_RUNTIME=ON -DCMAKE_INSTALL_PREFIX:PATH=install
cd build-StaticRuntime
call cmake --build . --target INSTALL --config Debug
call cmake --build . --target INSTALL --config RelWithDebInfo
call xcopy Debug\*.lib ..\protobuf\libs\StaticRuntime\%PLATFORM%\ /y
call xcopy Debug\*.pdb ..\protobuf\libs\StaticRuntime\%PLATFORM%\ /y
call xcopy Debug\*.exe ..\protobuf\libs\StaticRuntime\%PLATFORM%\ /y
call xcopy RelWithDebInfo\*.lib ..\protobuf\libs\StaticRuntime\%PLATFORM%\ /y
call xcopy RelWithDebInfo\*.pdb ..\protobuf\libs\StaticRuntime\%PLATFORM%\ /y
call xcopy RelWithDebInfo\*.exe ..\protobuf\libs\StaticRuntime\%PLATFORM%\ /y

cd %ROOT_PATH%
call rmdir /s /q build
call mkdir build
call cmake -S . -B build -A %PLATFORM% -G %BUILD_SYSTEM% -T %TOOLSET% -DCMAKE_SYSTEM_VERSION=%SDK_VERSION%
cd build
call cmake --build . --config Debug
call cmake --build . --config RelWithDebInfo
call xcopy %ROOT_PATH%\protobuf\libs\%PLATFORM%\*.dll build\Debug\ /y
call xcopy %ROOT_PATH%\protobuf\libs\%PLATFORM%\*.dll build\RelWithDebInfo\ /y

cd %ROOT_PATH%
call rmdir /s /q build-StaticRuntime
call mkdir build-StaticRuntime
call cmake -S . -B build-StaticRuntime -A %PLATFORM% -G %BUILD_SYSTEM% -T %TOOLSET% -DCMAKE_SYSTEM_VERSION=%SDK_VERSION% -DMSVC_STATIC_RUNTIME=ON -DBUILD_SHARED_LIBS=OFF
cd build-StaticRuntime
call cmake --build . --config Debug
call cmake --build . --config RelWithDebInfo