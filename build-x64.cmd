@echo off

SET PATH=%cd%\tools;%cd%\tools\cmake\bin;%PATH%

SET ROOT_PATH=%cd%

::protobuf
cd %ROOT_PATH%\protobuf
call git checkout v3.21.12
call rmdir /s /q build
call mkdir build
call cmake -S . -B build -A x64 -DCMAKE_CXX_STANDARD=14 -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB_DEFAULT=OFF -DCMAKE_INSTALL_PREFIX:PATH=install
cd build
call cmake --build . --target INSTALL --config Debug
call cmake --build . --target INSTALL --config RelWithDebInfo
call rmdir /s /q ..\libs\x64
call xcopy install\include ..\libs\includes\ /s /e /y
call xcopy Debug\*.lib ..\libs\x64\ /y
call xcopy Debug\*.pdb ..\libs\x64\ /y
call xcopy Debug\*.exe ..\libs\x64\ /y
call xcopy RelWithDebInfo\*.lib ..\libs\x64\ /y
call xcopy RelWithDebInfo\*.pdb ..\libs\x64\ /y
call xcopy RelWithDebInfo\*.exe ..\libs\x64\ /y
cd ..
call rmdir /s /q build