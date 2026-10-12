#include <windows.h>
#include <cstdio>
#include <fstream>
#include <string>

int wmain(int argc, wchar_t** argv) {
    if (argc != 4) return 1;
    std::ifstream beforeLog(argv[2], std::ios::binary | std::ios::ate);
    const auto start = beforeLog ? beforeLog.tellg() : std::streampos(0);
    beforeLog.close();
    const HMODULE dll = LoadLibraryW(argv[1]);
    if (!dll) { std::printf("LoadLibrary failed: %lu\n", GetLastError()); return 1; }
    using Status = DWORD (WINAPI*)(void*);
    const auto status = reinterpret_cast<Status>(GetProcAddress(dll, "VelvetSwapStatus"));
    const auto stop = reinterpret_cast<Status>(GetProcAddress(dll, "VelvetSwapStop"));
    if (!status || !stop) return 1;
    const auto deadline = GetTickCount64() + 10000;
    while (status(nullptr) == 0 && GetTickCount64() < deadline) Sleep(20);
    const DWORD result = status(nullptr);
    stop(nullptr);
    std::printf("DLL wrong-host status=%lu (expected 2); no game process used\n", result);
    // The observer pins its own module; process exit performs final teardown.
    std::ifstream log(argv[2], std::ios::binary);
    if (!log) return 1;
    log.seekg(start);
    const std::string lines((std::istreambuf_iterator<char>(log)), std::istreambuf_iterator<char>());
    char expected[256]{};
    const int converted = WideCharToMultiByte(CP_UTF8, 0, argv[3], -1,
        expected, sizeof(expected), nullptr, nullptr);
    return result == 2 && converted && lines.find(expected) != std::string::npos &&
        lines.find("game probes=0 hotkey actions=0") != std::string::npos &&
        lines.find("[HOST]") == std::string::npos ? 0 : 1;
}
