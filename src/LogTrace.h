#ifndef LOGTRACE_H
#define LOGTRACE_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#endif

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace LogTrace {
    inline char* GetLogStrA(const char* format, va_list args) {
        char* buffer = nullptr;

        __try {
            size_t len = (size_t)_vscprintf(format, args)
                + 1 // _vscprintf doesn't count terminating '\0'
                + 2 // \r\n
                ;

            buffer = (char*)calloc(len, sizeof(char));
            if (buffer) {
                size_t offset = 0;
                _vsnprintf_s(buffer + offset, len - offset, _TRUNCATE, format, args);
                offset = strlen(buffer);

                strncat_s(buffer + offset, len - offset, "\r\n", _TRUNCATE);
            }
        } __except (EXCEPTION_EXECUTE_HANDLER) {
        }

        return buffer;
    }

    inline void LogCurrentTime() {
        wchar_t timeBuffer[32];
        SYSTEMTIME sysTime;
        memset(&sysTime, 0, sizeof(SYSTEMTIME));
        GetSystemTime(&sysTime);

        _snwprintf_s(timeBuffer, _countof(timeBuffer), _TRUNCATE, L"[%04u/%02u/%02u %02u:%02u:%02u]", sysTime.wYear,
            sysTime.wMonth, sysTime.wDay, sysTime.wHour + 8, sysTime.wMinute, sysTime.wSecond);

#ifdef _CONSOLE
        wprintf(timeBuffer);
        OutputDebugStringW(timeBuffer);
#else
        OutputDebugStringW(timeBuffer);
#endif
    }

    inline void LogExA(const char* format, va_list args) {
        char* buffer = GetLogStrA(format, args);
        if (buffer) {

#ifdef _CONSOLE
            printf(buffer);
            OutputDebugStringA(buffer);
#else
            OutputDebugStringA(buffer);
#endif
            free(buffer);
        }
    }

    inline void LogA(const char* format, ...) {
#ifndef _DEBUG
        return;
#endif
        va_list args;
        va_start(args, format);

        LogExA(format, args);

        va_end(args);
    }

    inline void LogWithTimeA(const char* format, ...) {
#ifndef _DEBUG
        return;
#endif
        va_list args;
        va_start(args, format);

        LogCurrentTime();

        LogExA(format, args);

        va_end(args);
}

    // release模式也会打印日志
    inline void ForceLogA(const char* format, ...) {
        va_list args;
        va_start(args, format);

        LogExA(format, args);

        va_end(args);
    }

    // release模式也会打印日志
    inline void ForceLogWithTimeA(const char* format, ...) {
        va_list args;
        va_start(args, format);

        LogCurrentTime();

        LogExA(format, args);

        va_end(args);
    }

    inline wchar_t* GetLogStrW(const wchar_t* format, va_list args) {
        wchar_t* buffer = nullptr;

        __try {
            size_t len = (size_t)_vscwprintf(format, args)
                + 1 // _vscprintf doesn't count terminating '\0'
                + 2 // \r\n
                ;

            buffer = (wchar_t*)calloc(len, sizeof(wchar_t));
            if (buffer) {
                size_t offset = 0;
                _vsnwprintf_s(buffer + offset, len - offset, _TRUNCATE, format, args);
                offset = wcslen(buffer);

                wcsncat_s(buffer + offset, len - offset, L"\r\n", _TRUNCATE);
            }
        } __except (EXCEPTION_EXECUTE_HANDLER) {
        }

        return buffer;
    }

    inline void LogExW(const wchar_t* format, va_list args) {
        wchar_t* buffer = GetLogStrW(format, args);
        if (buffer) {
#ifdef _CONSOLE
            wprintf(buffer);
            OutputDebugStringW(buffer);
#else
            OutputDebugStringW(buffer);
#endif
            free(buffer);
        }
    }

    inline void LogW(const wchar_t* format, ...) {
#ifndef _DEBUG
        return;
#endif
        va_list args;
        va_start(args, format);

        LogExW(format, args);

        va_end(args);
    }

    inline void LogWithTimeW(const wchar_t* format, ...) {
#ifndef _DEBUG
        return;
#endif
        va_list args;
        va_start(args, format);

        LogCurrentTime();

        LogExW(format, args);

        va_end(args);
    }

    // release模式也会打印日志
    inline void ForceLogW(const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);

        LogExW(format, args);

        va_end(args);
    }

    // release模式也会打印日志
    inline void ForceLogWithTimeW(const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);

        LogCurrentTime();

        LogExW(format, args);

        va_end(args);
    }

    inline void LogToFileA(const char* logFile, const char* format, ...) {
        va_list args;
        va_start(args, format);

        char* buffer = GetLogStrA(format, args);
        if (buffer) {
            DWORD writeBytes = 0;
            HANDLE fileHandle = CreateFileA(logFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (fileHandle != INVALID_HANDLE_VALUE) {
                SetFilePointer(fileHandle, 0, 0, FILE_END);

                WriteFile(fileHandle, buffer, (DWORD)strlen(buffer), &writeBytes, NULL);
                CloseHandle(fileHandle);
            }

            free(buffer);
        }

        va_end(args);
    }

    inline void LogToFileW(const wchar_t* logFile, const wchar_t* format, ...) {
        const unsigned char head[] = { 0xFF, 0xFE };
        
        va_list args;
        va_start(args, format);

        wchar_t* buffer = GetLogStrW(format, args);
        if (buffer) {
            DWORD writeBytes = 0;
            HANDLE fileHandle = CreateFileW(logFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (fileHandle != INVALID_HANDLE_VALUE) {
                // 首次创建文件写入UTF16 BOM头
                if (0 == GetLastError()) {
                    WriteFile(fileHandle, head, (DWORD)sizeof(head), &writeBytes, NULL);
                }

                SetFilePointer(fileHandle, 0, 0, FILE_END);

                WriteFile(fileHandle, buffer, (DWORD)(wcslen(buffer) * sizeof(buffer[0])), &writeBytes, NULL);
                CloseHandle(fileHandle);

                free(buffer);
            }
        }

        va_end(args);
    }
}

#endif //LOGTRACE_H