// PipeServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

int main()
{
    do {
        std::wstring pipeName = LR"(\\.\pipe\{17842D3A-9729-4BCE-AF0C-12E10D3FB70F}-pipe)";
        std::wstring heartbeatEventName/* = LR"({3FE940AC-F7BD-4630-8D7B-2935B80EAFA6}-Heartbeat)"*/;

        bool stop = false;

        PipeWrapper pipeWrapper(pipeName, heartbeatEventName, PipeType::PipeServer);

        pipeWrapper.RegisterCallback(nullptr, [=](void* ctx)-> bool {
            return stop;
            }, [&](void* ctx) {
                stop = true;
            });

        if (!pipeWrapper.ConnectPipe(nullptr, -1)) {
            break;
        }

        HANDLE fileHandle = CreateFileW(LR"(D:\download\LLVM-15.0.7-win64.exe)", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (fileHandle != INVALID_HANDLE_VALUE) {
            DWORD readBytes = 0;
            int bufSize = 4096;
            char* buf = new char[bufSize];
            if (buf) {
                while (true) {
                    memset(buf, 0, bufSize);

                    if (!ReadFile(fileHandle, buf, bufSize, &readBytes, NULL) || readBytes == 0) {
                        break;
                    }

                    if (!pipeWrapper.Send(buf, readBytes)) {
                        break;
                    }
                }

                delete[] buf;
            }

            CloseHandle(fileHandle);
        }

        pipeWrapper.DisConnectPipe();

    } while (false);

    return 0;
}