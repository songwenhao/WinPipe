// PipeServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../CefPipeCmd.h"

PipeCmd::Cmd SendPipeCmd(PipeWrapper& pipeWrapper, const PipeCmd::Cmd& cmd, bool waitDone) {
    printf("[PipeServer] ==> send cmd, unique ID: %s action: %d content: %s\r\n", cmd.uniqueId.c_str(), cmd.action, cmd.content.c_str());
    return pipeWrapper.SendCmd(cmd, waitDone);
}

int main()
{
    do {
        std::wstring pipeName = LR"(\\.\pipe\{17842D3A-9729-4BCE-AF0C-12E10D3FB70F}-pipe)";
        std::wstring heartbeatEventName/* = LR"({3FE940AC-F7BD-4630-8D7B-2935B80EAFA6}-Heartbeat)"*/;

        bool stop = false;

        PipeWrapper pipeWrapper(pipeName, heartbeatEventName, PipeType::PipeServer);

        pipeWrapper.RegisterCallback(nullptr, [&](void* ctx, const PipeCmd::Cmd& cmd) {
            printf("[PipeServer] <== recv cmd, unique ID: %s action: %d content: %s\r\n", cmd.uniqueId.c_str(), cmd.action, cmd.content.c_str());

            CefPipeCmd::Action action = (CefPipeCmd::Action)cmd.action;
            switch (action) {
            case CefPipeCmd::Action::Unknown:
                break;
            case CefPipeCmd::Action::LoadUrl:
            {
                PipeCmd::Cmd sendCmd;
                sendCmd.action = cmd.action;
                sendCmd.uniqueId = cmd.uniqueId;
                SendPipeCmd(pipeWrapper, sendCmd, false);
            }
            break;
            case CefPipeCmd::Action::Reload:
                break;
            case CefPipeCmd::Action::RunJsCode:
                break;
            case CefPipeCmd::Action::GetCookie:
                break;
            case CefPipeCmd::Action::GetCurrentPageUrl:
            {
                PipeCmd::Cmd sendCmd;
                sendCmd.action = cmd.action;
                sendCmd.uniqueId = cmd.uniqueId;
                sendCmd.content = "http://www.google.com";
                SendPipeCmd(pipeWrapper, sendCmd, false);
            }
            break;
            case CefPipeCmd::Action::CheckDomIsExist:
                break;
            case CefPipeCmd::Action::MouseClick:
                break;
            case CefPipeCmd::Action::KeyboardInput:
                break;
            case CefPipeCmd::Action::ShowWindow:
                break;
            case CefPipeCmd::Action::HideWindow:
                break;
            case CefPipeCmd::Action::GetBoundingClientRect:
                break;
            case CefPipeCmd::Action::OnResponse:
                break;
            default:
                break;
            }

            }, [=](void* ctx)-> bool {
                return stop;
            }, [&](void* ctx) {
                stop = true;
            });

        if (!pipeWrapper.ConnectPipe(nullptr, -1)) {
            break;
        }

        while (!stop) {
            Sleep(100);
        }

        pipeWrapper.DisConnectPipe();

    } while (false);

    return 0;
}