#include "RuntimeProbe.h"
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <limits>

int main() {
    int failures = 0;
    const auto check = [&failures](bool condition, const char* label) {
        std::printf("%s %s\n", condition ? "PASS" : "FAIL", label);
        if (!condition) ++failures;
    };
    SYSTEM_INFO system{}; GetSystemInfo(&system);
    const auto page = system.dwPageSize;
    auto* memory = static_cast<unsigned char*>(VirtualAlloc(nullptr, 2 * page,
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    if (!memory) return 1;
    std::uint32_t input = 0x12345678, output = 0;
    std::memcpy(memory, &input, sizeof(input));
    check(RuntimeProbe::SafeRead(memory, &output, sizeof(output)) && output == input,
        "committed readable page");
    check(!RuntimeProbe::SafeRead(nullptr, &output, sizeof(output)), "null rejection");
    check(!RuntimeProbe::Readable(reinterpret_cast<void*>(0xfffffff0u), 32), "overflow rejection");
    DWORD old = 0;
    check(VirtualProtect(memory + page, page, PAGE_NOACCESS, &old) != 0, "protect test-owned page");
    check(!RuntimeProbe::SafeRead(memory + page - 2, &output, 4), "cross-page inaccessible read rejected");
    check(!RuntimeProbe::SafeRead(memory + page, &output, 4), "noaccess rejection");
    check(VirtualProtect(memory + page, page, PAGE_READWRITE | PAGE_GUARD, &old) != 0,
        "guard test-owned page");
    check(!RuntimeProbe::SafeRead(memory + page, &output, 4), "guard page rejected without consuming guard");
    MEMORY_BASIC_INFORMATION region{};
    VirtualQuery(memory + page, &region, sizeof(region));
    check((region.Protect & PAGE_GUARD) != 0, "guard preserved");
    check(VirtualFree(memory, 0, MEM_RELEASE) != 0, "release test-owned pages");
    check(!RuntimeProbe::SafeRead(memory, &output, sizeof(output)), "freed memory rejection");
    RuntimeProbe::Image fake;
    fake.base = 0x10000000; fake.size = 0x600000;
    std::uintptr_t rebased = 0;
    check(RuntimeProbe::Rebase(fake, 0x008ec408, 4, rebased) && rebased == 0x104ec408,
        "preferred-image rebasing");
    check(!RuntimeProbe::Rebase(fake, 0x003fffff, 4, rebased), "below preferred base rejected");
    check(!RuntimeProbe::InModule(fake, fake.base + fake.size - 2, 4), "module boundary rejection");
    const auto empty = RuntimeProbe::Capture(fake);
    check(empty.hostAddress == 0 && !empty.context.readable, "unverified image performs no game probe");
    // Synthetic layout in test-owned allocations only; this is not live-game evidence.
    auto* fixture = static_cast<unsigned char*>(VirtualAlloc(nullptr, RuntimeProbe::ExpectedImageSize,
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    auto* objects = static_cast<unsigned char*>(VirtualAlloc(nullptr, 2 * page,
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    if (!fixture || !objects) return 1;
    check(VirtualProtect(objects + page, page, PAGE_NOACCESS, &old) != 0,
        "protect synthetic object boundary page");
    RuntimeProbe::Image controlled;
    controlled.base = reinterpret_cast<std::uintptr_t>(fixture);
    controlled.size = RuntimeProbe::ExpectedImageSize;
    controlled.identityVerified = true; // Explicit fixture bypass, never part of the DLL.
    const auto put = [](unsigned char* at, std::uint32_t value) { std::memcpy(at, &value, 4); };
    const auto rva = [](std::uintptr_t va) { return va - RuntimeProbe::PreferredBase; };
    const auto pointer = [](const void* p) { return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p)); };
    auto* context = objects;
    auto* owner = objects + 128;
    auto* binding = objects + 256;
    put(fixture + rva(0x008ec40c), pointer(context));
    fixture[rva(0x008ec410)] = 1;
    put(context, pointer(fixture + rva(0x007de8cc)));
    put(context + 0x18, 0x12345678);
    put(fixture + rva(0x008ec5cc), pointer(owner + 0x14));
    put(owner + 0x34, pointer(binding));
    put(binding, pointer(fixture + rva(0x007e0434)));
    put(binding + 0x24, 0xabcdef01);
    const auto controlledSample = RuntimeProbe::Capture(controlled);
    check(controlledSample.context.readable && controlledSample.context.raw == pointer(context),
        "synthetic host/context pointer read");
    check(controlledSample.contextMatchesReferenceVtable && controlledSample.contextNamespacePayload.raw == 0x12345678,
        "namespace payload gated by exact known vtable");
    check(controlledSample.bindingMatchesCodeVtable && controlledSample.bindingScopeWord.raw == 0xabcdef01,
        "candidate owner/binding and CODE metadata read");
    check(controlledSample.embeddedReference.raw == pointer(binding + 0x24) &&
        !controlledSample.bindingScopeMatchesReferenceVtable &&
        !controlledSample.boundNamespacePayloadCandidate.readable,
        "embedded object address distinguished from wrong reference vtable; payload gated");
    auto* payload = objects + 512;
    put(payload, 0x87654321);
    put(binding + 0x24, pointer(fixture + rva(0x007de8cc)));
    put(binding + 0x24 + 0x18, pointer(payload));
    const auto boundScope = RuntimeProbe::Capture(controlled);
    check(boundScope.bindingScopeMatchesReferenceVtable &&
        boundScope.embeddedReference.raw == pointer(binding + 0x24) &&
        boundScope.boundNamespacePayloadCandidate.readable &&
        boundScope.boundNamespacePayloadCandidate.raw == pointer(payload) &&
        boundScope.boundNamespacePayloadFirstWord.readable &&
        boundScope.boundNamespacePayloadFirstWord.raw == 0x87654321,
        "exact rebased embedded REF_NAMESPACE gates its +0x18 payload read");
    put(binding + 0x24, 0x007de8cc);
    const auto unrebasedScope = RuntimeProbe::Capture(controlled);
    check(!unrebasedScope.bindingScopeMatchesReferenceVtable &&
        !unrebasedScope.boundNamespacePayloadCandidate.readable,
        "preferred vtable literal rejected when fixture image is rebased");
    put(binding + 0x24, pointer(fixture + rva(0x007de8cc)));
    put(binding + 0x3c, 0);
    const auto nullPayload = RuntimeProbe::Capture(controlled);
    check(nullPayload.boundNamespacePayloadCandidate.readable &&
        nullPayload.boundNamespacePayloadCandidate.raw == 0 &&
        !nullPayload.boundNamespacePayloadFirstWord.readable,
        "null bound payload recorded without following it");
    put(binding + 0x3c, 1);
    const auto badPayload = RuntimeProbe::Capture(controlled);
    check(badPayload.boundNamespacePayloadCandidate.readable &&
        !badPayload.boundNamespacePayloadFirstWord.readable,
        "unreadable bound payload rejected");
    auto* edgeBinding = objects + page - 0x3c;
    put(edgeBinding, pointer(fixture + rva(0x007e0434)));
    put(edgeBinding + 0x24, pointer(fixture + rva(0x007de8cc)));
    put(owner + 0x34, pointer(edgeBinding));
    const auto inaccessiblePayload = RuntimeProbe::Capture(controlled);
    check(inaccessiblePayload.bindingScopeMatchesReferenceVtable &&
        !inaccessiblePayload.boundNamespacePayloadCandidate.readable,
        "readable reference vtable cannot bypass inaccessible +0x18 payload");
    put(owner + 0x34, pointer(binding));
    put(context, 0);
    put(binding, 0);
    const auto wrongTypes = RuntimeProbe::Capture(controlled);
    check(!wrongTypes.contextNamespacePayload.readable && !wrongTypes.bindingScopeWord.readable,
        "unknown vtables never enable typed payload reads");
    check(!wrongTypes.embeddedReference.readable &&
        !wrongTypes.boundNamespacePayloadCandidate.readable,
        "wrong CODE vtable disables embedded reference observation");
    put(fixture + rva(0x008ec40c), 1);
    const auto invalidObject = RuntimeProbe::Capture(controlled);
    check(!invalidObject.contextVtable.readable, "unreadable candidate object rejected");
    VirtualFree(objects, 0, MEM_RELEASE);
    VirtualFree(fixture, 0, MEM_RELEASE);
    RuntimeProbe::Image actual;
    check(!RuntimeProbe::Initialize(actual) && !actual.identityVerified, "wrong process fails closed");
    check(actual.sha256[0] != '\0', "wrong-host SHA-256 retained");
    wchar_t tempRoot[MAX_PATH]{}, file[MAX_PATH]{};
    if (!GetTempPathW(_countof(tempRoot), tempRoot) || !GetTempFileNameW(tempRoot, L"vst", 0, file)) return 1;
    const HANDLE hashInput = CreateFileW(file, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    DWORD written = 0;
    if (hashInput == INVALID_HANDLE_VALUE || !WriteFile(hashInput, "abc", 3, &written, nullptr)) return 1;
    CloseHandle(hashInput);
    char hash[65]{};
    check(RuntimeProbe::HashFile(file, hash) && strcmp(hash,
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad") == 0,
        "SHA-256 standard vector");
    DeleteFileW(file);
    return failures ? 1 : 0;
}
