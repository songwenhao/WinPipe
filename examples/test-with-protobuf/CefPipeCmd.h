#pragma once

#include <cstdint>
#include <string>

namespace CefPipeCmd {

    enum class Action : std::int32_t {
        Unknown = -1,
        LoadUrl = 0,
        Reload,
        RunJsCode,
        GetCookie,
        GetCurrentPageUrl,
        CheckDomIsExist,
        MouseClick,
        KeyboardInput,
        ShowWindow,
        HideWindow,
        GetBoundingClientRect,
        OnResponse
    };

    static std::wstring CmdActionToWString(Action action) {
        std::wstring actionString;

        switch (action) {
        case Action::Unknown:
            actionString = L"Unknown";
            break;
        case Action::LoadUrl:
            actionString = L"LoadUrl";
            break;
        case Action::Reload:
            actionString = L"Reload";
            break;
        case Action::RunJsCode:
            actionString = L"RunJsCode";
            break;
        case Action::GetCookie:
            actionString = L"GetCookie";
            break;
        case Action::GetCurrentPageUrl:
            actionString = L"GetCurrentPageUrl";
            break;
        case Action::CheckDomIsExist:
            actionString = L"CheckDomIsExist";
            break;
        case Action::MouseClick:
            actionString = L"MouseClick";
            break;
        case Action::KeyboardInput:
            actionString = L"KeyboardInput";
            break;
        case Action::ShowWindow:
            actionString = L"ShowWindow";
            break;
        case Action::HideWindow:
            actionString = L"HideWindow";
            break;
        case Action::GetBoundingClientRect:
            actionString = L"GetBoundingClientRect";
            break;
        case Action::OnResponse:
            actionString = L"OnResponse";
            break;
        default:
            break;
        }

        return actionString;
    }

}