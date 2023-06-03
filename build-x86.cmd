@echo off

SET PATH=%cd%\tools;%cd%\tools\cmake\bin;%PATH%

SET ROOT_PATH=%cd%

::protobuf
cd %ROOT_PATH%\protobuf
call git checkout v3.21.12
call rmdir /s /q build
call mkdir build
call cmake -S . -B build -A Win32 -DBUILD_SHARED_LIBS=ON -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB_DEFAULT=OFF -DCMAKE_INSTALL_PREFIX:PATH=install
cd build
call cmake --build . --target INSTALL --config Debug
call cmake --build . --target INSTALL --config RelWithDebInfo
call rmdir /s /q ..\libs\Win32
call xcopy install\include ..\libs\includes\ /s /e /y
call xcopy Debug\*.dll ..\libs\Win32\ /y
call xcopy Debug\*.lib ..\libs\Win32\ /y
call xcopy Debug\*.pdb ..\libs\Win32\ /y
call xcopy Debug\*.exe ..\libs\Win32\ /y
call xcopy RelWithDebInfo\*.dll ..\libs\Win32\ /y
call xcopy RelWithDebInfo\*.lib ..\libs\Win32\ /y
call xcopy RelWithDebInfo\*.pdb ..\libs\Win32\ /y
call xcopy RelWithDebInfo\*.exe ..\libs\Win32\ /y
cd ..
call rmdir /s /q build-StaticRuntime
call mkdir build-StaticRuntime
call cmake -S . -B build-StaticRuntime -A Win32 -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB_DEFAULT=OFF -Dprotobuf_MSVC_STATIC_RUNTIME=ON -DCMAKE_INSTALL_PREFIX:PATH=install
cd build-StaticRuntime
call cmake --build . --target INSTALL --config Debug
call cmake --build . --target INSTALL --config RelWithDebInfo
call xcopy Debug\*.lib ..\protobuf\libs\StaticRuntime\Win32\ /y
call xcopy Debug\*.pdb ..\protobuf\libs\StaticRuntime\Win32\ /y
call xcopy Debug\*.exe ..\protobuf\libs\StaticRuntime\Win32\ /y
call xcopy RelWithDebInfo\*.lib ..\protobuf\libs\StaticRuntime\Win32\ /y
call xcopy RelWithDebInfo\*.pdb ..\protobuf\libs\StaticRuntime\Win32\ /y
call xcopy RelWithDebInfo\*.exe ..\protobuf\libs\StaticRuntime\Win32\ /y
cd %ROOT_PATH%
call rmdir /s /q build
call mkdir build
call cmake -S . -B build -A Win32
call xcopy %ROOT_PATH%\protobuf\libs\Win32\*.dll build\Debug\ /y
call xcopy %ROOT_PATH%\protobuf\libs\Win32\*.dll build\RelWithDebInfo\ /y
cd ..
call rmdir /s /q build-StaticRuntime
call mkdir build-StaticRuntime
call cmake -S . -B build-StaticRuntime -A Win32 -DMSVC_STATIC_RUNTIME=ON -DBUILD_SHARED_LIBS=OFF
cd build-StaticRuntime
call cmake --build . --config Debug
call cmake --build . --config RelWithDebInfo