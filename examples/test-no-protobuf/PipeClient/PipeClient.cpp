// PipeClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

int main()
{
    do {
        std::wstring pipeName = LR"(\\.\pipe\{17842D3A-9729-4BCE-AF0C-12E10D3FB70F}-pipe)";
        std::wstring heartbeatEventName/* = LR"({3FE940AC-F7BD-4630-8D7B-2935B80EAFA6}-Heartbeat)"*/;

        bool stop = false;

        PipeWrapper pipeWrapper(pipeName, heartbeatEventName, PipeType::PipeClient);

        pipeWrapper.RegisterCallback(nullptr, [=](void* ctx)-> bool {
            return stop;
            }, [&](void* ctx) {
                stop = true;
            });

        if (!pipeWrapper.ConnectPipe(nullptr, -1)) {
            break;
        }

        HANDLE fileHandle = CreateFileW(LR"(E:\LLVM-15.0.7-win64.exe)", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
        if (fileHandle != INVALID_HANDLE_VALUE) {
            DWORD writeBytes = 0;
            int bufSize = 4096 * 2;
            char* buf = new char[bufSize];
            if (buf) {
                std::uint32_t recvSize = 0;
                while (true) {
                    memset(buf, 0, bufSize);

                    recvSize = pipeWrapper.Recv(buf, bufSize);
                    if (!recvSize) {
                        break;
                    }

                    if (!WriteFile(fileHandle, buf, recvSize, &writeBytes, NULL) || writeBytes == 0) {
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