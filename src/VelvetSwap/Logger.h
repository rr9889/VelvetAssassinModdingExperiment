#pragma once
#include <windows.h>
namespace Logger {
// Worker-thread only. The configured directory already exists under the build tree.
bool Open(const wchar_t* mainExecutablePath) noexcept;
void Line(const char* format, ...) noexcept;
void Close() noexcept;
}
