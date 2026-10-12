#include "RuntimeProbe.h"
#include <windows.h>
#include <cstdio>
#include <cstring>

namespace {
unsigned char* mutationAddresses[3]{};
std::uint32_t mutationValues[3]{};
unsigned char* unreadablePage = nullptr;
void MutateOwnedFixture() noexcept {
    for (int i = 0; i < 3; ++i)
        if (mutationAddresses[i]) std::memcpy(mutationAddresses[i], &mutationValues[i], 4);
    if (unreadablePage) {
        DWORD old = 0;
        VirtualProtect(unreadablePage, 0x1000, PAGE_NOACCESS, &old);
    }
}
}

int main() {
    int failures = 0;
    const auto check = [&failures](bool condition, const char* label) {
        std::printf("%s %s\n", condition ? "PASS" : "FAIL", label);
        if (!condition) ++failures;
    };
    SYSTEM_INFO system{}; GetSystemInfo(&system);
    const auto page = system.dwPageSize;
    auto* imageBytes = static_cast<unsigned char*>(VirtualAlloc(nullptr, RuntimeProbe::ExpectedImageSize,
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    auto* objects = static_cast<unsigned char*>(VirtualAlloc(nullptr, 2 * page,
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    auto* entries = static_cast<unsigned char*>(VirtualAlloc(nullptr, 0x5600,
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    if (!imageBytes || !objects || !entries) return 1;
    DWORD old = 0;
    check(VirtualProtect(objects + page, page, PAGE_NOACCESS, &old) != 0, "fixture guard boundary");
    const auto ptr = [](const void* p) { return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p)); };
    const auto put = [](unsigned char* p, std::uint32_t x) { std::memcpy(p, &x, 4); };
    const auto rva = [](std::uintptr_t va) { return va - RuntimeProbe::PreferredBase; };
    RuntimeProbe::Image image;
    image.base = reinterpret_cast<std::uintptr_t>(imageBytes);
    image.size = RuntimeProbe::ExpectedImageSize;
    image.identityVerified = true; // Owned fixture only; real initialization must hash the executable.
    auto* code = objects;
    auto* context = objects + 0x200;
    auto* otherReceiver = objects + 0x400;
    auto* control = objects + 0x600;
    auto* table = objects + 0x700;
    auto* value = objects + 0x800;
    auto* payload = objects + 0xa00;
    auto* redirect = objects + 0xc00;
    auto* slot = entries + 0x55f0;
    put(imageBytes + rva(0x008ec40c), ptr(context));
    put(imageBytes + rva(0x008e9b88), ptr(code));
    put(imageBytes + rva(0x0081fc9c), ptr(imageBytes + rva(0x00459940)));
    put(code, ptr(imageBytes + rva(0x007e0434)));
    put(code + 0x18, ptr(table)); put(code + 0x1c, ptr(table + 0x20));
    put(table + 0x18, ptr(context));
    const auto setupReceiver = [&](unsigned char* r) {
        put(r, ptr(imageBytes + rva(0x0081fc04)));
        put(r + 0x90, 0); put(r + 0x9c, ptr(control));
        put(r + 0xc0, ptr(entries)); put(r + 0xc4, ptr(entries + 0x5600));
        put(r + 0xcc, 0); put(r + 0x2c, 0);
    };
    setupReceiver(context); setupReceiver(otherReceiver); setupReceiver(redirect);
    put(control + 0x14, 0);
    put(slot + 0x0c, ptr(value));
    put(value, ptr(imageBytes + rva(0x007de8cc)));
    put(value + 0x18, ptr(payload)); put(payload, 0x87654321);
    const auto capture = [&]() { return RuntimeProbe::Capture(image); };
    auto s = capture();
    check(s.r0.codeMatches && s.r0.tableBoundsValid && s.r0.tableCount.raw == 1,
        "valid CODE reference table and exact rebased vtable");
    check(s.r0.record0.raw == ptr(table) && s.r0.receiver.raw == ptr(context), "record zero receiver extraction");
    check(s.r0.receiverReadable && s.r0.comparisonKnown && s.r0.equalsHostContext && s.r0.consistent,
        "R0 equals host context by pointer equality");
    check(s.r0.matchesAccessor && s.r0.mode.raw == 0, "exact indexed accessor gate");
    check(s.r0.slot.boundsValid && s.r0.slot.count.raw == 0x560 && s.r0.slot.record.raw == ptr(slot),
        "safe index 055f count stride and record calculation");
    check(s.r0.slot.safeRead && s.r0.slot.consistent && s.r0.slot.storedPointer.raw == ptr(value),
        "ordinary direct stored value observation");
    check(s.r0.slot.valueMatchesReferenceVtable && s.r0.slot.referenceCandidate.raw == ptr(payload) &&
        s.r0.slot.referenceFirstWord.raw == 0x87654321, "exact slot REF_NAMESPACE type gates payload metadata");
    check(!s.embeddedReference.readable && !s.boundNamespacePayloadCandidate.readable,
        "historical CODE scope probe disabled by default");
    put(table + 0x18, ptr(otherReceiver)); s = capture();
    check(s.r0.comparisonKnown && !s.r0.equalsHostContext && s.r0.slot.safeRead,
        "distinct R0 independently validated without host alias assumption");
    put(table + 0x18, 0); s = capture();
    check(s.r0.receiver.readable && s.r0.receiver.raw == 0 && !s.r0.receiverReadable &&
        !s.r0.comparisonKnown && !s.r0.slot.storedPointer.readable, "null R0 never followed");
    put(table + 0x18, 1); s = capture();
    check(!s.r0.receiverReadable && !s.r0.slot.storedPointer.readable, "unreadable R0 rejected");
    put(table + 0x18, ptr(context));
    put(code, 0); s = capture();
    check(!s.r0.tableBegin.readable && !s.r0.receiver.readable, "wrong CODE vtable prevents table observation");
    put(code, ptr(imageBytes + rva(0x007e0434)));
    put(code + 0x1c, ptr(table)); s = capture();
    check(!s.r0.tableBoundsValid && !s.r0.receiver.readable, "empty reference table rejected");
    put(code + 0x1c, ptr(table - 0x20)); s = capture();
    check(!s.r0.tableBoundsValid && !s.r0.receiver.readable, "reversed reference bounds rejected");
    put(code + 0x1c, ptr(table + 0x21)); s = capture();
    check(!s.r0.tableBoundsValid && !s.r0.receiver.readable, "incompatible reference stride rejected");
    put(code + 0x18, ptr(objects + page - 0x10)); put(code + 0x1c, ptr(objects + page + 0x10));
    s = capture();
    check(!s.r0.tableBoundsValid && !s.r0.receiver.readable, "reference record crossing noaccess page rejected");
    put(code + 0x18, ptr(table)); put(code + 0x1c, ptr(table + 0x20));
    put(imageBytes + rva(0x0081fc9c), 0); s = capture();
    check(!s.r0.matchesAccessor && !s.r0.mode.readable && !s.r0.slot.storedPointer.readable,
        "unverified accessor prevents layout and slot reads");
    put(imageBytes + rva(0x0081fc9c), ptr(imageBytes + rva(0x00459940)));
    put(context + 0x9c, 0); s = capture();
    check(!s.r0.slot.flags.readable && !s.r0.slot.storedPointer.readable,
        "unresolved control prevents slot reads");
    put(context + 0x9c, ptr(control));
    put(context + 0xc0, 0); s = capture();
    check(!s.r0.slot.boundsValid && !s.r0.slot.storedPointer.readable, "null storage begin rejected");
    put(context + 0xc0, ptr(entries)); put(context + 0xc4, ptr(entries + 0x55f0)); s = capture();
    check(!s.r0.slot.boundsValid && s.r0.slot.count.raw == 0x55f && !s.r0.slot.storedPointer.readable,
        "index equal to count rejected");
    put(context + 0xc4, ptr(entries - 0x10)); s = capture();
    check(!s.r0.slot.boundsValid && !s.r0.slot.storedPointer.readable, "reversed slot storage rejected");
    put(context + 0xc4, ptr(entries + 0x5601)); s = capture();
    check(!s.r0.slot.boundsValid && !s.r0.slot.storedPointer.readable, "incompatible slot stride rejected");
    put(context + 0xc0, 0x10); put(context + 0xc4, 0xfffffff0); s = capture();
    check(!s.r0.slot.boundsValid && !s.r0.slot.storedPointer.readable, "signed-count overflow range rejected");
    put(context + 0xc0, ptr(entries)); put(context + 0xc4, ptr(entries + 0x5600));
    put(control + 0x14, 1); slot[4] = 0; s = capture();
    check(s.r0.slot.flags.readable && !s.r0.slot.storedPointer.readable && !s.r0.slot.safeRead,
        "unresolved mirror branch never reads stored value pointer");
    slot[4] = 2; s = capture();
    check(s.r0.slot.safeRead && s.r0.slot.storedPointer.raw == ptr(value), "flag bit2 validates direct return branch");
    put(control + 0x14, 0); slot[4] = 0;
    put(context + 0xcc, 4); s = capture();
    check(s.r0.slot.safeRead, "mode4 nonnull local direct value");
    put(slot + 0x0c, 0); s = capture();
    check(!s.r0.slot.safeRead && s.r0.slot.storedPointer.readable && s.r0.slot.storedPointer.raw == 0 &&
        std::strcmp(s.r0.slot.branch, "FALLBACK_REQUIRED_UNRESOLVED") == 0,
        "mode4 local null recorded without guessing fallback result");
    put(context + 0xcc, 6); s = capture();
    check(!s.r0.slot.safeRead, "mode6 fallback also unresolved");
    put(context + 0xcc, 0); s = capture();
    check(s.r0.slot.safeRead && !s.r0.slot.valueVtable.readable, "ordinary null value recorded without following it");
    put(slot + 0x0c, ptr(value));
    put(context + 0xcc, 7); put(context + 0x90, ptr(redirect)); put(context + 0x9c, 0);
    put(context + 0xc0, 0); put(context + 0xc4, 0); s = capture();
    check(s.r0.slot.safeRead && s.r0.slot.storageReceiver.raw == ptr(redirect) &&
        s.r0.slot.begin.raw == ptr(entries), "mode7 uses redirected storage and bypasses original control");
    put(context + 0x90, 0); s = capture();
    check(!s.r0.slot.storedPointer.readable && !s.r0.slot.safeRead, "null mode7 storage receiver rejected");
    setupReceiver(context);
    put(value, 0); s = capture();
    check(s.r0.slot.safeRead && !s.r0.slot.valueMatchesReferenceVtable &&
        !s.r0.slot.referenceCandidate.readable, "unknown slot type never enables typed payload read");
    put(value, ptr(imageBytes + rva(0x007de8cc)));
    const auto changedCapture = [&](unsigned char* address, std::uint32_t newValue) {
        mutationAddresses[0] = address; mutationValues[0] = newValue;
        RuntimeProbe::OwnedFixtureBeforeRecheck = &MutateOwnedFixture;
        auto result = capture();
        RuntimeProbe::OwnedFixtureBeforeRecheck = nullptr;
        mutationAddresses[0] = nullptr;
        return result;
    };
    s = capture();
    check(std::strcmp(s.r0.snapshotClass, "FULL_CHAIN_STABLE") == 0 && s.r0.receiverRevalidated &&
        s.r0.slot.safeRead, "fully stable chain independently revalidated and accepted");
    s = changedCapture(imageBytes + rva(0x008e9b88), 0);
    check(std::strcmp(s.r0.snapshotClass, "RECEIVER_STABLE_CODE_TURNOVER") == 0 &&
        !s.r0.rechecks[0].same && s.r0.rechecks[6].same && s.r0.rechecks[7].same &&
        s.r0.rechecks[8].same && s.r0.rechecks[9].same,
        "current CODE turnover isolated from stable receiver identity");
    check(s.r0.receiverRevalidated && s.r0.mode.readable && s.r0.slot.safeRead && s.r0.slot.consistent,
        "storage observed after independently revalidated CODE turnover, including final receiver recheck");
    put(imageBytes + rva(0x008e9b88), ptr(code));
    auto* secondTable = objects + 0xe00;
    put(secondTable + 0x18, ptr(context));
    mutationAddresses[1] = code + 0x1c; mutationValues[1] = ptr(secondTable + 0x40);
    s = changedCapture(code + 0x18, ptr(secondTable));
    mutationAddresses[1] = nullptr;
    check(std::strcmp(s.r0.snapshotClass, "RECEIVER_STABLE_CODE_TURNOVER") == 0 &&
        !s.r0.rechecks[2].same && !s.r0.rechecks[3].same && !s.r0.rechecks[4].same &&
        !s.r0.rechecks[5].same && s.r0.rechecks[6].same && s.r0.slot.safeRead,
        "valid reference table turnover preserves independently stable host receiver");
    put(code + 0x18, ptr(table)); put(code + 0x1c, ptr(table + 0x20));
    s = changedCapture(table + 0x18, ptr(otherReceiver));
    check(std::strcmp(s.r0.snapshotClass, "RECEIVER_INVALIDATED") == 0 && !s.r0.rechecks[6].same &&
        !s.r0.mode.readable && !s.r0.slot.storedPointer.readable, "R0 pointer change rejects storage");
    put(table + 0x18, ptr(context));
    s = changedCapture(imageBytes + rva(0x008ec40c), ptr(otherReceiver));
    check(!s.r0.rechecks[7].same && !s.r0.receiverRevalidated && !s.r0.mode.readable,
        "host context change rejects storage");
    put(imageBytes + rva(0x008ec40c), ptr(context));
    s = changedCapture(context, 0);
    check(!s.r0.rechecks[8].same && !s.r0.receiverRevalidated && !s.r0.mode.readable,
        "receiver vtable change rejects storage");
    setupReceiver(context);
    s = changedCapture(imageBytes + rva(0x0081fc9c), 0);
    check(!s.r0.rechecks[9].same && !s.r0.receiverRevalidated && !s.r0.mode.readable,
        "receiver getter change rejects storage");
    put(imageBytes + rva(0x0081fc9c), ptr(imageBytes + rva(0x00459940)));
    unreadablePage = objects; s = changedCapture(nullptr, 0); unreadablePage = nullptr;
    check(!s.r0.rechecks[8].after.readable && !s.r0.receiverRevalidated && !s.r0.mode.readable &&
        !s.r0.slot.storedPointer.readable, "R0 becoming unreadable rejects storage and still logs all rechecks");
    VirtualProtect(objects, page, PAGE_READWRITE, &old);
    put(table + 0x18, ptr(otherReceiver));
    s = changedCapture(imageBytes + rva(0x008e9b88), 0);
    check(!s.r0.equalsHostContext && !s.r0.receiverRevalidated && !s.r0.mode.readable,
        "CODE turnover is not accepted without independent host VMACHINE alias");
    put(table + 0x18, ptr(context)); put(imageBytes + rva(0x008e9b88), ptr(code));
    s = changedCapture(code + 0x1c, ptr(table));
    check(!s.r0.afterTableBoundsValid && !s.r0.receiverRevalidated && !s.r0.mode.readable,
        "invalidated after-reference bounds reject even a stable host pointer");
    put(code + 0x1c, ptr(table + 0x20));
    put(slot + 4, 0xaabbcc02); put(control + 0x14, 1); s = capture();
    check(s.r0.slot.safeRead && s.r0.slot.flags.raw == 0xaabbcc02,
        "full DWORD record flags observed; bit2 alone selects direct branch");
    put(slot + 4, 0); put(control + 0x14, 0);
    put(context + 0xcc, 4); put(context + 0x9c, 0); put(slot + 0x0c, 0); s = capture();
    check(std::strcmp(s.r0.slot.branch, "FALLBACK_REQUIRED_UNRESOLVED") == 0 &&
        !s.r0.slot.safeRead && !s.r0.slot.valueVtable.readable,
        "mode4 local NULL selects unresolved fallback before common control dereference");
    setupReceiver(context); put(context + 0xcc, 6); put(slot + 0x0c, ptr(value));
    put(control + 0x14, 1); s = capture();
    check(s.r0.slot.localPointerForFallbackCheck.readable && !s.r0.slot.storedPointer.readable &&
        !s.r0.slot.safeRead && !s.r0.slot.valueVtable.readable,
        "mode6 nonnull local predicate does not enable unresolved mirror value observation");
    setupReceiver(context); put(control + 0x14, 0);
    image.identityVerified = false; s = capture();
    check(!s.r0.codeVtable.readable && !s.r0.receiver.readable && !s.r0.slot.storedPointer.readable,
        "unverified executable performs no R0 or slot probe");
    VirtualFree(entries, 0, MEM_RELEASE);
    VirtualFree(objects, 0, MEM_RELEASE);
    VirtualFree(imageBytes, 0, MEM_RELEASE);
    return failures ? 1 : 0;
}
