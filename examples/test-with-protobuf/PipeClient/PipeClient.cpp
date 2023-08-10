// PipeClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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

        PipeWrapper pipeWrapper(pipeName, heartbeatEventName, PipeType::PipeClient);

        pipeWrapper.RegisterCallback(nullptr, [](void* ctx, const PipeCmd::Cmd& cmd) {
            printf("[PipeClient] <== recv cmd, unique ID: %s action: %d content: %s\r\n", cmd.unique_id().c_str(), cmd.action(), cmd.content().c_str());
            }, [=](void* ctx)-> bool {
            return stop;
            }, [&](void* ctx) {
                stop = true;
            });

        if (!pipeWrapper.ConnectPipe(nullptr, -1)) {
            break;
        }

        Cmd sendCmd;
        sendCmd.set_action(std::int32_t(CefPipeCmd::Action::LoadUrl));
        sendCmd.set_content("http://www.google.com");
        Cmd resultCmd = pipeWrapper.SendCmd(sendCmd);

        Sleep(1000);

        while (true) {
            sendCmd.Clear();
            sendCmd.set_action(std::int32_t(CefPipeCmd::Action::GetCurrentPageUrl));
            resultCmd = pipeWrapper.SendCmd(sendCmd);

            const auto& url = resultCmd.content();
            if (url.empty()) {
                break;
            }

            Sleep(1000);
        }

        pipeWrapper.DisConnectPipe();

    } while (false);

    return 0;
}