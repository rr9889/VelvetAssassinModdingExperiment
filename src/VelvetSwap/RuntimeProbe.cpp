#include "RuntimeProbe.h"
#include <psapi.h>
#include <bcrypt.h>
#include <cstring>
#include <cwchar>
#include <limits>

static_assert(sizeof(void*) == 4, "Only x86 is supported.");
#ifdef VELVETSWAP_OWNED_FIXTURE
void (*RuntimeProbe::OwnedFixtureBeforeRecheck)() noexcept = nullptr;
#endif
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
bool Same(RuntimeProbe::Value a, RuntimeProbe::Value b) noexcept {
    return a.readable && b.readable && a.raw == b.raw;
}
RuntimeProbe::FieldRecheck Recheck(const char* name, RuntimeProbe::Value before,
                                  RuntimeProbe::Value after) noexcept {
    return {name, before, after, Same(before, after)};
}
RuntimeProbe::Value Getter(const RuntimeProbe::Image& image, RuntimeProbe::Value vtable) noexcept {
    return vtable.readable && vtable.raw && RuntimeProbe::InModule(image, vtable.raw, 0x9c)
        ? ReadMember(vtable, 0x98) : RuntimeProbe::Value{};
}
bool Record(RuntimeProbe::Value begin, RuntimeProbe::Value end, std::uint32_t stride,
            std::uint32_t index, RuntimeProbe::Value& count,
            RuntimeProbe::Value& address) noexcept {
    // Native SAR count applies to well-formed positive ranges; reject malformed ranges.
    if (!begin.readable || !end.readable || !begin.raw || end.raw < begin.raw) return false;
    const auto bytes = end.raw - begin.raw;
    if (bytes > 0x7fffffffu || bytes % stride) return false;
    count = {bytes / stride, true};
    if (index >= count.raw) return false;
    // Within [begin,end), so these additions cannot wrap; full record must be readable.
    const auto target = static_cast<std::uintptr_t>(begin.raw) + index * stride;
    if (!RuntimeProbe::Readable(reinterpret_cast<const void*>(target), stride)) return false;
    address = {static_cast<std::uint32_t>(target), true};
    return true;
}
void ObserveR0(const RuntimeProbe::Image& image, RuntimeProbe::Snapshot& snapshot) noexcept {
    auto& r = snapshot.r0;
    auto& slot = r.slot;
    std::uintptr_t codeVtable = 0, accessor = 0, referenceVtable = 0, vmachineVtable = 0;
    if (!RuntimeProbe::Rebase(image, 0x007e0434, 4, codeVtable) ||
        !RuntimeProbe::Rebase(image, 0x00459940, 1, accessor) ||
        !RuntimeProbe::Rebase(image, 0x007de8cc, 4, referenceVtable) ||
        !RuntimeProbe::Rebase(image, 0x0081fc04, 0x9c, vmachineVtable)) return;
    r.codeVtable = ReadMember(snapshot.currentCode, 0);
    r.codeMatches = r.codeVtable.readable && r.codeVtable.raw == codeVtable;
    if (!r.codeMatches) { r.reason = "current_CODE_type_unverified"; return; }
    // ev_784062... @ 462242..46226b: CODE ref records are 0x20 bytes, receiver at +0x18.
    r.tableBegin = ReadMember(snapshot.currentCode, 0x18);
    r.tableEnd = ReadMember(snapshot.currentCode, 0x1c);
    r.tableBoundsValid = Record(r.tableBegin, r.tableEnd, 0x20, 0, r.tableCount, r.record0);
    if (!r.tableBoundsValid) { r.reason = "reference_record0_bounds_invalid"; return; }
    r.receiver = ReadMember(r.record0, 0x18);
    r.vtable = ReadMember(r.receiver, 0);
    r.receiverReadable = r.vtable.readable;
    r.comparisonKnown = r.receiverReadable && r.receiver.raw && snapshot.context.readable &&
        snapshot.context.raw && snapshot.contextVtable.readable;
    r.equalsHostContext = r.comparisonKnown && r.receiver.raw == snapshot.context.raw;
    if (!r.receiverReadable) { r.reason = "R0_null_or_unreadable"; return; }
    r.virtual98 = Getter(image, r.vtable);
    r.matchesAccessor = r.virtual98.readable && r.virtual98.raw == accessor;
#ifdef VELVETSWAP_OWNED_FIXTURE
    if (RuntimeProbe::OwnedFixtureBeforeRecheck) RuntimeProbe::OwnedFixtureBeforeRecheck();
#endif
    // Recheck the CAPTURED CODE's table, not a different current CODE's table. The
    // current-CODE global is separately observed. Never short-circuit these reads.
    const auto codeAfter = ReadAt(image, 0x008e9b88);
    const auto codeVtableAfter = ReadMember(snapshot.currentCode, 0);
    const auto tableAfter = ReadMember(snapshot.currentCode, 0x18);
    const auto endAfter = ReadMember(snapshot.currentCode, 0x1c);
    RuntimeProbe::Value countAfter, recordAfter;
    r.afterTableBoundsValid = Record(tableAfter, endAfter, 0x20, 0, countAfter, recordAfter);
    const auto receiverAfter = r.afterTableBoundsValid ? ReadMember(recordAfter, 0x18)
                                                      : RuntimeProbe::Value{};
    const auto hostAfter = Read32(snapshot.hostAddress + 4);
    // Independent receiver metadata comes from captured R0, not transient CODE.
    const auto vtableAfter = ReadMember(r.receiver, 0);
    const auto getterAfter = Getter(image, vtableAfter);
    auto& checks = r.rechecks;
    checks[0] = Recheck("code", snapshot.currentCode, codeAfter);
    checks[1] = Recheck("code_vtable", r.codeVtable, codeVtableAfter);
    checks[2] = Recheck("ref_table", r.tableBegin, tableAfter);
    checks[3] = Recheck("ref_end", r.tableEnd, endAfter);
    checks[4] = Recheck("ref_count", r.tableCount, countAfter);
    checks[5] = Recheck("record0", r.record0, recordAfter);
    checks[6] = Recheck("r0", r.receiver, receiverAfter);
    checks[7] = Recheck("host", snapshot.context, hostAfter);
    checks[8] = Recheck("r0_vtable", r.vtable, vtableAfter);
    checks[9] = Recheck("virtual98", r.virtual98, getterAfter);
    r.recheckAttempted = true;
    r.consistent = true;
    for (const auto& check : checks) r.consistent = r.consistent && check.same;
    const bool independentReceiver = checks[6].same && checks[7].same && checks[8].same &&
        checks[9].same && r.receiver.raw && r.matchesAccessor &&
        RuntimeProbe::Readable(reinterpret_cast<const void*>(r.receiver.raw), 4);
    // Snapshot-lifetime rule: an independently unchanged host VMACHINE remains a
    // guarded read target after current CODE turns over. This proves no lock,
    // ownership, atomicity or protection against ABA; final storage rechecks follow.
    const bool turnoverAllowed = independentReceiver && r.equalsHostContext &&
        r.vtable.raw == vmachineVtable && vtableAfter.raw == vmachineVtable && checks[1].same &&
        r.afterTableBoundsValid;
    r.receiverRevalidated = independentReceiver && (r.consistent || turnoverAllowed);
    r.snapshotClass = !r.receiverRevalidated ? "RECEIVER_INVALIDATED" :
        (r.consistent ? "FULL_CHAIN_STABLE" : "RECEIVER_STABLE_CODE_TURNOVER");
    if (!r.receiverRevalidated) {
        // Preserve initial raw equality and each failure instead of erasing them.
        r.reason = "receiver_snapshot_rejected"; slot.reason = r.reason; return;
    }
    // All following offsets are instruction-confirmed for exactly this accessor.
    r.mode = ReadMember(r.receiver, 0xcc);
    r.field90 = ReadMember(r.receiver, 0x90);
    r.field9c = ReadMember(r.receiver, 0x9c);
    r.storageBegin = ReadMember(r.receiver, 0xc0);
    r.storageEnd = ReadMember(r.receiver, 0xc4);
    r.fallback2c = ReadMember(r.receiver, 0x2c);
    if (!r.mode.readable) { r.reason = "mode_unreadable"; slot.reason = r.reason; return; }
    r.reason = "R0_accessor_metadata_observed";
    const bool redirected = r.mode.raw == 7;
    const bool fallbackMode = r.mode.raw == 4 || r.mode.raw == 6;
    slot.storageReceiver = redirected ? r.field90 : r.receiver;
    slot.branch = redirected ? "MODE7_REDIRECT" :
        ((r.mode.raw == 4 || r.mode.raw == 6) ? "MODE4_6_LOCAL_CANDIDATE" : "OWN_STORAGE_COMMON");
    slot.begin = redirected ? ReadMember(slot.storageReceiver, 0xc0) : r.storageBegin;
    slot.end = redirected ? ReadMember(slot.storageReceiver, 0xc4) : r.storageEnd;
    if (!redirected) {
        r.control14 = ReadMember(r.field9c, 0x14);
        if (!fallbackMode && !r.control14.readable) {
            slot.branch = "UNRESOLVED"; slot.reason = "field_9c_control14_unreadable"; return;
        }
    }
    slot.boundsValid = Record(slot.begin, slot.end, 0x10, 0x055f, slot.count, slot.record);
    if (!slot.boundsValid) { slot.reason = "slot_bounds_or_record_readability_invalid"; return; }
    slot.flags = ReadMember(slot.record, 4);
    if (!slot.flags.readable) { slot.reason = "record_flags_unreadable"; return; }
    // Modes 4/6 test local NULL before entering the common control/flag path.
    if (fallbackMode) {
        slot.localPointerForFallbackCheck = ReadMember(slot.record, 0x0c);
        if (!slot.localPointerForFallbackCheck.readable) { slot.reason = "stored_pointer_field_unreadable"; return; }
        if (!slot.localPointerForFallbackCheck.raw) {
            slot.storedPointer = slot.localPointerForFallbackCheck;
            slot.branch = "FALLBACK_REQUIRED_UNRESOLVED";
            slot.reason = "fallback_receiver_accessor_result_unresolved";
            return; // Local NULL is not the fallback value; do not inspect its metadata.
        }
    }
    if (!redirected && !r.control14.readable) {
        slot.branch = "UNRESOLVED"; slot.reason = "field_9c_control14_unreadable"; return;
    }
    if (!redirected && r.control14.raw && !(slot.flags.raw & 2)) {
        // Do not invoke RTTI cast/virtual unwrap or read a value under an unresolved branch.
        slot.branch = "MIRROR_OR_FALLBACK_UNRESOLVED";
        slot.reason = "checked_mirror_unwrap_contract_required";
        return;
    }
    slot.storedPointer = ReadMember(slot.record, 0x0c);
    if (!slot.storedPointer.readable) { slot.reason = "stored_pointer_field_unreadable"; return; }
    slot.safeRead = true;
    slot.reason = "validated_direct_return_branch";
    // Receiver/storage identity is independently rechecked. Current CODE/table
    // turnover cannot invalidate this accepted receiver solely by changing CODE.
    const auto finalVtable = ReadMember(r.receiver, 0);
    auto& final = slot.rechecks;
    final[0] = Recheck("host", snapshot.context, Read32(snapshot.hostAddress + 4));
    final[1] = Recheck("r0_vtable", r.vtable, finalVtable);
    final[2] = Recheck("virtual98", r.virtual98, Getter(image, finalVtable));
    final[3] = Recheck("mode", r.mode, ReadMember(r.receiver, 0xcc));
    final[4] = Recheck("storage_begin", slot.begin, ReadMember(slot.storageReceiver, 0xc0));
    final[5] = Recheck("storage_end", slot.end, ReadMember(slot.storageReceiver, 0xc4));
    final[6] = Recheck("stored_pointer", slot.storedPointer, ReadMember(slot.record, 0x0c));
    final[7] = Recheck("flags", slot.flags, ReadMember(slot.record, 4));
    final[8] = Recheck(redirected ? "field90" : "field9c", redirected ? r.field90 : r.field9c,
        ReadMember(r.receiver, redirected ? 0x90 : 0x9c));
    if (!redirected) final[9] = Recheck("control14", r.control14, ReadMember(r.field9c, 0x14));
    slot.recheckAttempted = true;
    slot.consistent = true;
    for (std::size_t i = 0; i < (redirected ? 9u : 10u); ++i)
        slot.consistent = slot.consistent && final[i].same;
    if (!slot.consistent) {
        slot.safeRead = false; slot.reason = "selection_changed_during_sample"; return;
    }
    if (!slot.safeRead) return;
    slot.valueVtable = ReadMember(slot.storedPointer, 0);
    slot.valueMatchesReferenceVtable = slot.valueVtable.readable && slot.valueVtable.raw == referenceVtable;
    if (slot.valueMatchesReferenceVtable) {
        // This is slot-value type metadata, NOT the disabled CODE+0x24 experiment.
        slot.referenceCandidate = ReadMember(slot.storedPointer, 0x18);
        slot.referenceFirstWord = ReadMember(slot.referenceCandidate, 0);
    }
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
RuntimeProbe::Snapshot RuntimeProbe::Capture(const Image& image, bool historicalScopeFixture) noexcept {
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
        if (historicalScopeFixture && snapshot.bindingMatchesCodeVtable && snapshot.ownerBinding.raw &&
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
    ObserveR0(image, snapshot);
    return snapshot;
}
