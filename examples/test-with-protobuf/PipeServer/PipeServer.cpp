// PipeServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../CefPipeCmd.h"

int main()
{
    do {
        using namespace PipeCmd;

        std::wstring pipeName = LR"(\\.\pipe\{17842D3A-9729-4BCE-AF0C-12E10D3FB70F}-pipe)";
        std::wstring heartbeatEventName/* = LR"({3FE940AC-F7BD-4630-8D7B-2935B80EAFA6}-Heartbeat)"*/;

        bool stop = false;

        PipeWrapper pipeWrapper(pipeName, heartbeatEventName, PipeType::PipeServer);

        pipeWrapper.RegisterCallback(nullptr, [&](void* ctx, const PipeCmd::Cmd& cmd) {
            printf("[PipeServer] <== recv cmd, unique ID: %s action: %d content: %s\r\n", cmd.unique_id().c_str(), cmd.action(), cmd.content().c_str());

            CefPipeCmd::Action action = (CefPipeCmd::Action)cmd.action();
            switch (action) {
            case CefPipeCmd::Action::Unknown:
                break;
            case CefPipeCmd::Action::LoadUrl:
            {
                Cmd sendCmd;
                sendCmd.set_action(cmd.action());
                sendCmd.set_unique_id(cmd.unique_id());
                pipeWrapper.SendCmd(sendCmd, false);
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
                Cmd sendCmd;
                sendCmd.set_action(cmd.action());
                sendCmd.set_unique_id(cmd.unique_id());
                sendCmd.set_content("http://www.google.com");
                pipeWrapper.SendCmd(sendCmd, false);
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