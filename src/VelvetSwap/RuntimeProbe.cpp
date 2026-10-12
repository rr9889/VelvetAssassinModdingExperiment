#include "RuntimeProbe.h"
#include <psapi.h>
#include <bcrypt.h>
#include <cstring>
#include <cwchar>
#include <limits>

static_assert(sizeof(void*) == 4, "Only x86 is supported.");
namespace {
bool CopySeh(const void* source, void* destination, std::size_t bytes) noexcept {
    // No C++ objects requiring unwinding in this SEH boundary.
    __try { std::memcpy(destination, source, bytes); return true; }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
RuntimeProbe::Value Read32(std::uintptr_t address) noexcept {
    RuntimeProbe::Value value;
    value.readable = RuntimeProbe::SafeRead(reinterpret_cast<const void*>(address),
        &value.raw, sizeof(value.raw));
    return value;
}
RuntimeProbe::Value ReadAt(const RuntimeProbe::Image& image,
                           std::uintptr_t preferredAddress) noexcept {
    std::uintptr_t address = 0;
    return RuntimeProbe::Rebase(image, preferredAddress, 4, address) ? Read32(address)
                                                                  : RuntimeProbe::Value{};
}
RuntimeProbe::Value ReadMember(RuntimeProbe::Value pointer, std::uintptr_t offset) noexcept {
    if (!pointer.readable || !pointer.raw ||
        pointer.raw > (std::numeric_limits<std::uintptr_t>::max)() - offset) return {};
    return Read32(static_cast<std::uintptr_t>(pointer.raw) + offset);
}
}
bool RuntimeProbe::Readable(const void* address, std::size_t bytes) noexcept {
    const auto start = reinterpret_cast<std::uintptr_t>(address);
    if (!start || !bytes || start > (std::numeric_limits<std::uintptr_t>::max)() - bytes)
        return false;
    const auto end = start + bytes;
    auto cursor = start;
    while (cursor < end) {
        MEMORY_BASIC_INFORMATION region{};
        if (!VirtualQuery(reinterpret_cast<const void*>(cursor), &region, sizeof(region)))
            return false;
        if (region.State != MEM_COMMIT || (region.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
            return false;
        const DWORD protection = region.Protect & 0xff;
        if (protection != PAGE_READONLY && protection != PAGE_READWRITE &&
            protection != PAGE_WRITECOPY && protection != PAGE_EXECUTE_READ &&
            protection != PAGE_EXECUTE_READWRITE && protection != PAGE_EXECUTE_WRITECOPY)
            return false;
        const auto regionStart = reinterpret_cast<std::uintptr_t>(region.BaseAddress);
        if (region.RegionSize > (std::numeric_limits<std::uintptr_t>::max)() - regionStart)
            return false;
        const auto next = regionStart + region.RegionSize;
        if (next <= cursor) return false;
        cursor = next;
    }
    return true;
}
bool RuntimeProbe::SafeRead(const void* source, void* destination, std::size_t bytes) noexcept {
    if (!destination || !Readable(source, bytes)) return false;
    return CopySeh(source, destination, bytes);
}
bool RuntimeProbe::InModule(const Image& image, std::uintptr_t address,
                            std::size_t bytes) noexcept {
    if (!image.base || !bytes || address < image.base) return false;
    const auto offset = address - image.base;
    return offset < image.size && bytes <= image.size - offset;
}
bool RuntimeProbe::Rebase(const Image& image, std::uintptr_t preferredAddress,
                          std::size_t bytes, std::uintptr_t& address) noexcept {
    address = 0;
    if (preferredAddress < PreferredBase) return false;
    const auto rva = preferredAddress - PreferredBase;
    if (image.base > (std::numeric_limits<std::uintptr_t>::max)() - rva) return false;
    const auto candidate = image.base + rva;
    if (!InModule(image, candidate, bytes)) return false;
    address = candidate;
    return true;
}
bool RuntimeProbe::HashFile(const wchar_t* path, char (&hex)[65]) noexcept {
    hex[0] = '\0';
    HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE,
        nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;
    BCRYPT_ALG_HANDLE algorithm = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    unsigned char digest[32]{};
    unsigned char buffer[16384]{};
    bool ok = BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM,
        nullptr, 0) >= 0;
    if (ok) ok = BCryptCreateHash(algorithm, &hash, nullptr, 0, nullptr, 0, 0) >= 0;
    DWORD bytes = 0;
    while (ok) {
        if (!ReadFile(file, buffer, sizeof(buffer), &bytes, nullptr)) { ok = false; break; }
        if (!bytes) break;
        ok = BCryptHashData(hash, buffer, bytes, 0) >= 0;
    }
    if (ok) ok = BCryptFinishHash(hash, digest, sizeof(digest), 0) >= 0;
    if (hash) BCryptDestroyHash(hash);
    if (algorithm) BCryptCloseAlgorithmProvider(algorithm, 0);
    CloseHandle(file);
    if (ok) {
        constexpr char digits[] = "0123456789abcdef";
        for (std::size_t i = 0; i < sizeof(digest); ++i) {
            hex[2 * i] = digits[digest[i] >> 4];
            hex[2 * i + 1] = digits[digest[i] & 15];
        }
        hex[64] = '\0';
    }
    return ok;
}
bool RuntimeProbe::Initialize(Image& image) noexcept {
    image = {};
    const HMODULE mainModule = GetModuleHandleW(nullptr);
    MODULEINFO info{};
    // Current-process pseudo-handle only; never OpenProcess or external memory access.
    if (!mainModule || !GetModuleInformation(GetCurrentProcess(), mainModule, &info, sizeof(info))) {
        strcpy_s(image.rejection, "module information unavailable"); return false;
    }
    image.base = reinterpret_cast<std::uintptr_t>(info.lpBaseOfDll);
    image.size = info.SizeOfImage;
    const DWORD length = GetModuleFileNameW(mainModule, image.path, _countof(image.path));
    if (!length || length >= _countof(image.path)) {
        strcpy_s(image.rejection, "executable path unavailable"); return false;
    }
    const wchar_t* filename = wcsrchr(image.path, L'\\');
    filename = filename ? filename + 1 : image.path;
    // Hash even a wrong host for an attributable rejection log, but do not probe it.
    if (!HashFile(image.path, image.sha256)) {
        strcpy_s(image.rejection, "executable SHA-256 unavailable"); return false;
    }
    if (_wcsicmp(filename, L"replay.exe") != 0) {
        strcpy_s(image.rejection, "host is not replay.exe"); return false;
    }
    if (strcmp(image.sha256, ExpectedSha256) != 0) {
        strcpy_s(image.rejection, "executable SHA-256 mismatch"); return false;
    }
    IMAGE_DOS_HEADER dos{};
    if (!InModule(image, image.base, sizeof(dos)) ||
        !SafeRead(reinterpret_cast<const void*>(image.base), &dos, sizeof(dos)) ||
        dos.e_magic != IMAGE_DOS_SIGNATURE || dos.e_lfanew < 0) {
        strcpy_s(image.rejection, "invalid DOS header"); return false;
    }
    const auto ntOffset = static_cast<std::uintptr_t>(dos.e_lfanew);
    if (ntOffset >= image.size) { strcpy_s(image.rejection, "invalid PE offset"); return false; }
    IMAGE_NT_HEADERS32 nt{};
    if (!InModule(image, image.base + ntOffset, sizeof(nt)) ||
        !SafeRead(reinterpret_cast<const void*>(image.base + ntOffset), &nt, sizeof(nt))) {
        strcpy_s(image.rejection, "unreadable PE header"); return false;
    }
    image.timestamp = nt.FileHeader.TimeDateStamp;
    if (nt.Signature != IMAGE_NT_SIGNATURE || nt.FileHeader.Machine != IMAGE_FILE_MACHINE_I386 ||
        nt.OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC ||
        nt.OptionalHeader.ImageBase != PreferredBase || image.size != ExpectedImageSize ||
        nt.OptionalHeader.SizeOfImage != ExpectedImageSize || image.timestamp != ExpectedTimestamp) {
        strcpy_s(image.rejection, "loaded PE identity mismatch"); return false;
    }
    image.identityVerified = true;
    return true;
}
RuntimeProbe::Snapshot RuntimeProbe::Capture(const Image& image) noexcept {
    Snapshot snapshot;
    if (!image.identityVerified) return snapshot;
    // ev_320646... / ev_bafd882...: host is an object at this VA, not a pointer slot.
    if (Rebase(image, 0x008ec408, 9, snapshot.hostAddress)) {
        snapshot.context = Read32(snapshot.hostAddress + 4);
        unsigned char enabled = 0;
        snapshot.hostEnabled.readable = SafeRead(
            reinterpret_cast<const void*>(snapshot.hostAddress + 8), &enabled, 1);
        snapshot.hostEnabled.raw = enabled;
    }
    snapshot.contextVtable = ReadMember(snapshot.context, 0);
    std::uintptr_t expectedReferenceVtable = 0;
    if (Rebase(image, 0x007de8cc, 4, expectedReferenceVtable))
        snapshot.contextMatchesReferenceVtable = snapshot.contextVtable.readable &&
            snapshot.contextVtable.raw == expectedReferenceVtable;
    if (snapshot.contextMatchesReferenceVtable)
        snapshot.contextNamespacePayload = ReadMember(snapshot.context, 0x18);
    // Known namespace virtual +0x98 entry: read metadata only, never invoke it.
    if (snapshot.contextVtable.readable && snapshot.contextVtable.raw &&
        InModule(image, snapshot.contextVtable.raw, 0x9c))
        snapshot.contextGetValueEntry = ReadMember(snapshot.contextVtable, 0x98);
    snapshot.invocationDepth = ReadAt(image, 0x008e9b84);
    snapshot.currentCode = ReadAt(image, 0x008e9b88);
    snapshot.callbackHead = ReadAt(image, 0x008ec5cc);
    // ev_d5481...: decompiler-supported list link -> owner adjustment. Candidate only.
    if (snapshot.callbackHead.readable && snapshot.callbackHead.raw >= 0x14) {
        snapshot.candidateOwner = snapshot.callbackHead.raw - 0x14;
        Value owner;
        owner.raw = static_cast<std::uint32_t>(snapshot.candidateOwner);
        owner.readable = true;
        snapshot.ownerBinding = ReadMember(owner, 0x34);
        snapshot.bindingVtable = ReadMember(snapshot.ownerBinding, 0);
        std::uintptr_t codeVtable = 0;
        if (Rebase(image, 0x007e0434, 4, codeVtable))
            snapshot.bindingMatchesCodeVtable = snapshot.bindingVtable.readable &&
                snapshot.bindingVtable.raw == codeVtable;
        if (snapshot.bindingMatchesCodeVtable && snapshot.ownerBinding.raw &&
            snapshot.ownerBinding.raw <= (std::numeric_limits<std::uintptr_t>::max)() - 0x24) {
            // CODE+0x24 is the reference object itself, not a pointer to it.
            snapshot.embeddedReference.raw = snapshot.ownerBinding.raw + 0x24;
            snapshot.embeddedReference.readable = true; // Computed address, not dereferenced.
            snapshot.bindingScopeWord = ReadMember(snapshot.ownerBinding, 0x24);
            snapshot.bindingScopeMatchesReferenceVtable = expectedReferenceVtable &&
                snapshot.bindingScopeWord.readable &&
                snapshot.bindingScopeWord.raw == expectedReferenceVtable;
            if (snapshot.bindingScopeMatchesReferenceVtable) {
                // ev_1121e... validates +0x18 only for this exact REF_NAMESPACE type.
                snapshot.boundNamespacePayloadCandidate = ReadMember(snapshot.embeddedReference, 0x18);
                // Raw pointed-to metadata only; this does not identify a namespace or camera.
                snapshot.boundNamespacePayloadFirstWord = ReadMember(snapshot.boundNamespacePayloadCandidate, 0);
            }
        }
    }
    return snapshot;
}
