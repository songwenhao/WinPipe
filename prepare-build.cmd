@echo off

set use_proxy=0
set http_proxy_cmd=""
set https_proxy_cmd=""
if /I "%1"=="proxy" (
	set use_proxy=1
	set http_proxy_cmd= -e "http_proxy=http://%2"
	set https_proxy_cmd= -e "https_proxy=https://%2"
	echo "proxy=%2%"
)

SET ROOT_PATH=%cd%

cd tools

::cmake
call wget.exe%https_proxy_cmd% https://github.com/Kitware/CMake/releases/download/v3.26.3/cmake-3.26.3-windows-i386.zip
call 7z.exe x cmake-3.26.3-windows-i386.zip -o.
call rename cmake-3.26.3-windows-i386 cmake

cd %ROOT_PATH%