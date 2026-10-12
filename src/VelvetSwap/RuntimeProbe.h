#pragma once
#include <windows.h>
#include <cstdint>
#include <cstddef>

namespace RuntimeProbe {
inline constexpr std::uintptr_t PreferredBase = 0x00400000;
inline constexpr DWORD ExpectedImageSize = 0x00585e84;
inline constexpr DWORD ExpectedTimestamp = 0x49ef137b;
inline constexpr char ExpectedSha256[] = "9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132";
struct Image {
    std::uintptr_t base = 0;
    DWORD size = 0;
    DWORD timestamp = 0;
    bool identityVerified = false;
    wchar_t path[32768]{};
    char sha256[65]{};
    char rejection[192]{};
};
struct Value {
    std::uint32_t raw = 0;
    bool readable = false;
};
struct Snapshot {
    std::uintptr_t hostAddress = 0;
    Value context;
    Value contextVtable;
    Value contextGetValueEntry;
    Value contextNamespacePayload;
    Value hostEnabled;
    Value invocationDepth;
    Value currentCode;
    Value callbackHead;
    Value ownerBinding;
    Value bindingVtable;
    Value bindingScopeWord;
    std::uintptr_t candidateOwner = 0;
    bool bindingMatchesCodeVtable = false;
    bool contextMatchesReferenceVtable = false;
    // Deliberately no slot/camera/outfit fields: no verified native layout exists.
};
bool Readable(const void* address, std::size_t bytes) noexcept;
bool SafeRead(const void* source, void* destination, std::size_t bytes) noexcept;
bool InModule(const Image& image, std::uintptr_t address, std::size_t bytes) noexcept;
bool Rebase(const Image& image, std::uintptr_t preferredAddress,
            std::size_t bytes, std::uintptr_t& address) noexcept;
bool HashFile(const wchar_t* path, char (&hex)[65]) noexcept;
bool Initialize(Image& image) noexcept;
Snapshot Capture(const Image& image) noexcept;
}
