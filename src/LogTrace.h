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
    inline char* GetLogStr(const char* format, va_list args) {
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

    inline wchar_t* GetLogStr(const wchar_t* format, va_list args) {
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

    inline void GetCurrentTimeString(char buffer[], size_t bufferSizeInCharacters) {
        SYSTEMTIME sysTime;
        memset(&sysTime, 0, sizeof(SYSTEMTIME));
        GetLocalTime(&sysTime);

        _snprintf_s(buffer, bufferSizeInCharacters, _TRUNCATE, "[%04u/%02u/%02u %02u:%02u:%02u]", sysTime.wYear,
            sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    }

    inline void GetCurrentTimeString(wchar_t buffer[], size_t bufferSizeInCharacters) {
        SYSTEMTIME sysTime;
        memset(&sysTime, 0, sizeof(SYSTEMTIME));
        GetLocalTime(&sysTime);

        _snwprintf_s(buffer, bufferSizeInCharacters, _TRUNCATE, L"[%04u/%02u/%02u %02u:%02u:%02u]", sysTime.wYear,
            sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    }

    inline void LogEx(const char* format, va_list args) {
        char* buffer = GetLogStr(format, args);
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

    inline void LogEx(const wchar_t* format, va_list args) {
        wchar_t* buffer = GetLogStr(format, args);
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

    inline void Log(const char* format, ...) {
#ifndef _DEBUG
        return;
#endif
        va_list args;
        va_start(args, format);

        LogEx(format, args);

        va_end(args);
    }

    inline void Log(const wchar_t* format, ...) {
#ifndef _DEBUG
        return;
#endif
        va_list args;
        va_start(args, format);

        LogEx(format, args);

        va_end(args);
    }

    inline void LogWithTime(const char* format, ...) {
#ifndef _DEBUG
        return;
#endif
        va_list args;
        va_start(args, format);

        wchar_t buffer[32];
        memset(buffer, 0, sizeof(buffer));
        GetCurrentTimeString(buffer, _countof(buffer));

#ifdef _CONSOLE
        wprintf(buffer);
        OutputDebugStringW(buffer);
#else
        OutputDebugStringW(buffer);
#endif

        LogEx(format, args);

        va_end(args);
}

    inline void LogWithTime(const wchar_t* format, ...) {
#ifndef _DEBUG
        return;
#endif
        va_list args;
        va_start(args, format);

        wchar_t buffer[32];
        memset(buffer, 0, sizeof(buffer));
        GetCurrentTimeString(buffer, _countof(buffer));

#ifdef _CONSOLE
        wprintf(buffer);
        OutputDebugStringW(buffer);
#else
        OutputDebugStringW(buffer);
#endif

        LogEx(format, args);

        va_end(args);
    }

    // release模式也会打印日志
    inline void ForceLog(const char* format, ...) {
        va_list args;
        va_start(args, format);

        LogEx(format, args);

        va_end(args);
    }

    // release模式也会打印日志
    inline void ForceLog(const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);

        LogEx(format, args);

        va_end(args);
    }

    // release模式也会打印日志
    inline void ForceLogWithTime(const char* format, ...) {
        va_list args;
        va_start(args, format);

        wchar_t buffer[32];
        memset(buffer, 0, sizeof(buffer));
        GetCurrentTimeString(buffer, _countof(buffer));

#ifdef _CONSOLE
        wprintf(buffer);
        OutputDebugStringW(buffer);
#else
        OutputDebugStringW(buffer);
#endif

        LogEx(format, args);

        va_end(args);
    }

    // release模式也会打印日志
    inline void ForceLogWithTime(const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);

        wchar_t buffer[32];
        memset(buffer, 0, sizeof(buffer));
        GetCurrentTimeString(buffer, _countof(buffer));

#ifdef _CONSOLE
        wprintf(buffer);
        OutputDebugStringW(buffer);
#else
        OutputDebugStringW(buffer);
#endif

        LogEx(format, args);

        va_end(args);
    }

    inline void WriteLogToFile(const char* logFile, const char* logContent, size_t logContentSize) {
        DWORD writeBytes = 0;
        HANDLE fileHandle = CreateFileA(logFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle != INVALID_HANDLE_VALUE) {
            SetFilePointer(fileHandle, 0, 0, FILE_END);

            WriteFile(fileHandle, logContent, (DWORD)logContentSize, &writeBytes, NULL);
            CloseHandle(fileHandle);
        }
    }

    inline void WriteLogToFile(const wchar_t* logFile, const char* logContent, size_t logContentSize) {
        DWORD writeBytes = 0;
        HANDLE fileHandle = CreateFileW(logFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle != INVALID_HANDLE_VALUE) {
            SetFilePointer(fileHandle, 0, 0, FILE_END);

            WriteFile(fileHandle, logContent, (DWORD)logContentSize, &writeBytes, NULL);
            CloseHandle(fileHandle);
        }
    }

    inline void WriteLogToFile(const char* logFile, const wchar_t* logContent, size_t logContentSize) {
        const unsigned char head[] = { 0xFF, 0xFE };
        DWORD writeBytes = 0;
        HANDLE fileHandle = CreateFileA(logFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle != INVALID_HANDLE_VALUE) {
            // 首次创建文件写入UTF16 BOM头
            if (0 == GetLastError()) {
                WriteFile(fileHandle, head, (DWORD)sizeof(head), &writeBytes, NULL);
            }

            SetFilePointer(fileHandle, 0, 0, FILE_END);

            WriteFile(fileHandle, logContent, (DWORD)logContentSize, &writeBytes, NULL);

            CloseHandle(fileHandle);
        }
    }

    inline void WriteLogToFile(const wchar_t* logFile, const wchar_t* logContent, size_t logContentSize) {
        const unsigned char head[] = { 0xFF, 0xFE };
        DWORD writeBytes = 0;
        HANDLE fileHandle = CreateFileW(logFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle != INVALID_HANDLE_VALUE) {
            // 首次创建文件写入UTF16 BOM头
            if (0 == GetLastError()) {
                WriteFile(fileHandle, head, (DWORD)sizeof(head), &writeBytes, NULL);
            }

            SetFilePointer(fileHandle, 0, 0, FILE_END);

            WriteFile(fileHandle, logContent, (DWORD)logContentSize, &writeBytes, NULL);

            CloseHandle(fileHandle);
        }
    }

    inline void LogToFile(const char* logFile, const char* format, ...) {
        va_list args;
        va_start(args, format);

        char* logContent = GetLogStr(format, args);
        if (logContent) {
            WriteLogToFile(logFile, logContent, strlen(logContent));

            free(logContent);
        }

        va_end(args);
    }

    inline void LogToFile(const wchar_t* logFile, const char* format, ...) {
        va_list args;
        va_start(args, format);

        char* logContent = GetLogStr(format, args);
        if (logContent) {
            WriteLogToFile(logFile, logContent, strlen(logContent));

            free(logContent);
        }

        va_end(args);
    }

    inline void LogToFile(const char* logFile, const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);

        wchar_t* logContent = GetLogStr(format, args);
        if (logContent) {
            WriteLogToFile(logFile, logContent, wcslen(logContent) * sizeof(logContent[0]));

            free(logContent);
        }

        va_end(args);
    }

    inline void LogToFile(const wchar_t* logFile, const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);

        wchar_t* logContent = GetLogStr(format, args);
        if (logContent) {
            WriteLogToFile(logFile, logContent, wcslen(logContent) * sizeof(logContent[0]));

            free(logContent);
        }

        va_end(args);
    }

    inline void LogToFileWithTime(const char* logFile, const char* format, ...) {
        va_list args;
        va_start(args, format);

        char* logContent = GetLogStr(format, args);
        if (logContent) {
            char timeBuffer[32];
            memset(timeBuffer, 0, sizeof(timeBuffer));
            GetCurrentTimeString(timeBuffer, _countof(timeBuffer));
            WriteLogToFile(logFile, timeBuffer, strlen(timeBuffer));

            WriteLogToFile(logFile, logContent, strlen(logContent));

            free(logContent);
        }

        va_end(args);
    }

    inline void LogToFileWithTime(const wchar_t* logFile, const char* format, ...) {
        va_list args;
        va_start(args, format);

        char* logContent = GetLogStr(format, args);
        if (logContent) {
            char timeBuffer[32];
            memset(timeBuffer, 0, sizeof(timeBuffer));
            GetCurrentTimeString(timeBuffer, _countof(timeBuffer));
            WriteLogToFile(logFile, timeBuffer, strlen(timeBuffer));

            WriteLogToFile(logFile, logContent, strlen(logContent));

            free(logContent);
        }

        va_end(args);
    }

    inline void LogToFileWithTime(const char* logFile, const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);

        wchar_t* logContent = GetLogStr(format, args);
        if (logContent) {
            wchar_t timeBuffer[32];
            memset(timeBuffer, 0, sizeof(timeBuffer));
            GetCurrentTimeString(timeBuffer, _countof(timeBuffer));
            WriteLogToFile(logFile, timeBuffer, wcslen(timeBuffer) * sizeof(timeBuffer[0]));

            WriteLogToFile(logFile, logContent, wcslen(logContent) * sizeof(logContent[0]));

            free(logContent);
        }

        va_end(args);
    }

    inline void LogToFileWithTime(const wchar_t* logFile, const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);

        wchar_t* logContent = GetLogStr(format, args);
        if (logContent) {
            wchar_t timeBuffer[32];
            memset(timeBuffer, 0, sizeof(timeBuffer));
            GetCurrentTimeString(timeBuffer, _countof(timeBuffer));
            WriteLogToFile(logFile, timeBuffer, wcslen(timeBuffer) * sizeof(timeBuffer[0]));

            WriteLogToFile(logFile, logContent, wcslen(logContent) * sizeof(logContent[0]));

            free(logContent);
        }

        va_end(args);
    }

}

#endif //LOGTRACE_H