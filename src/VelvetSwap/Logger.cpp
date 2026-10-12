#include "Logger.h"
#include "BuildConfig.h"
#include <cstdio>
#include <cstdarg>
#include <cwchar>
#include <cstdlib>

namespace {
HANDLE logHandle = INVALID_HANDLE_VALUE;
}
bool Logger::Open(const wchar_t* mainExecutablePath) noexcept {
    wchar_t logPath[32768]{};
    wchar_t gameDirectory[32768]{};
    const DWORD count = GetFullPathNameW(VELVETSWAP_LOG_DIRECTORY L"/VelvetSwap.log",
        static_cast<DWORD>(_countof(logPath)), logPath, nullptr);
    if (!count || count >= _countof(logPath)) return false;
    wcscpy_s(gameDirectory, mainExecutablePath);
    wchar_t* lastSlash = wcsrchr(gameDirectory, L'\\');
    if (!lastSlash) return false;
    lastSlash[1] = L'\0';
    // Fail closed if configured output would be under the game's installation.
    if (_wcsnicmp(logPath, gameDirectory, wcslen(gameDirectory)) == 0) return false;
    logHandle = CreateFileW(logPath, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    return logHandle != INVALID_HANDLE_VALUE;
}
void Logger::Line(const char* format, ...) noexcept {
    if (logHandle == INVALID_HANDLE_VALUE) return;
    char message[2048]{};
    va_list args;
    va_start(args, format);
    _vsnprintf_s(message, _countof(message), _TRUNCATE, format, args);
    va_end(args);
    SYSTEMTIME now{};
    GetSystemTime(&now);
    char line[2304]{};
    const int length = _snprintf_s(line, _countof(line), _TRUNCATE,
        "[%04u-%02u-%02uT%02u:%02u:%02u.%03uZ] [observer_tid=%lu] %s\r\n",
        now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
        now.wMilliseconds, GetCurrentThreadId(), message);
    if (length > 0) {
        DWORD written = 0;
        WriteFile(logHandle, line, static_cast<DWORD>(length), &written, nullptr);
        FlushFileBuffers(logHandle);
    }
}
void Logger::Close() noexcept {
    if (logHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(logHandle);
        logHandle = INVALID_HANDLE_VALUE;
    }
}
