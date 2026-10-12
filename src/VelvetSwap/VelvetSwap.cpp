#include "Logger.h"
#include "RuntimeProbe.h"
#include <windows.h>
#include <cstdio>
#include <cstring>

namespace {
HMODULE dllModule = nullptr;
volatile LONG stopRequested = 0;
// 0=starting, 1=observing, 2=rejected/failed, 3=stopped.
volatile LONG status = 0;
FILETIME loadTime{};

const char* Describe(RuntimeProbe::Value value, char (&text)[32]) noexcept {
    if (value.readable) sprintf_s(text, "0x%08lX", static_cast<unsigned long>(value.raw));
    else strcpy_s(text, "UNRESOLVED");
    return text;
}
void LogR0(const RuntimeProbe::Snapshot& s, const char* reason) noexcept {
    const auto& r = s.r0;
    const auto& slot = r.slot;
    char a[32]{}, b[32]{}, c[32]{}, d[32]{}, e[32]{}, f[32]{}, g[32]{}, h[32]{}, i[32]{}, j[32]{};
    Logger::Line("[CODE_R0] reason=%s code=%s code_vtable=%s matches_CODE=%u ref_table=%s ref_end=%s ref_count=%s ref_bounds_valid=%u record_index=0 record_address=%s receiver_R0=%s receiver_readable=%u host_context=%s equals_host_context=%u comparison=%s snapshot_class=%s receiver_revalidated=%u status=%s",
        reason, Describe(s.currentCode, a), Describe(r.codeVtable, b), r.codeMatches ? 1u : 0u,
        Describe(r.tableBegin, c), Describe(r.tableEnd, d), Describe(r.tableCount, e),
        r.tableBoundsValid ? 1u : 0u, Describe(r.record0, f), Describe(r.receiver, g),
        r.receiverReadable ? 1u : 0u, Describe(s.context, h), r.equalsHostContext ? 1u : 0u,
        r.comparisonKnown ? (r.equalsHostContext ? "EQUAL" : "DIFFERENT") : "UNAVAILABLE",
        r.snapshotClass, r.receiverRevalidated ? 1u : 0u, r.reason);
    for (const auto& check : r.rechecks) {
        if (!r.recheckAttempted) break;
        Logger::Line("[R0_RECHECK] reason=%s field=%s before=%s after=%s same=%u before_readable=%u after_readable=%u snapshot_class=%s after_ref_bounds_valid=%u table_source=captured_CODE receiver_metadata_source=captured_R0",
            reason, check.name, Describe(check.before, a), Describe(check.after, b), check.same ? 1u : 0u,
            check.before.readable ? 1u : 0u, check.after.readable ? 1u : 0u,
            r.snapshotClass, r.afterTableBoundsValid ? 1u : 0u);
    }
    Logger::Line("[R0_RECEIVER] reason=%s r0=%s vtable=%s virtual_98=%s matches_FUN_00459940=%u mode=%s field_90=%s field_9c=%s control_14=%s storage_begin=%s storage_end=%s fallback_2c=%s",
        reason, Describe(r.receiver, a), Describe(r.vtable, b), Describe(r.virtual98, c),
        r.matchesAccessor ? 1u : 0u, Describe(r.mode, d), Describe(r.field90, e),
        Describe(r.field9c, f), Describe(r.control14, g), Describe(r.storageBegin, h),
        Describe(r.storageEnd, i), Describe(r.fallback2c, j));
    Logger::Line("[R0_STORAGE_STATE] reason=%s r0=%s snapshot_class=%s mode=%s field_90=%s field_9c=%s control_14=%s storage_begin=%s storage_end=%s fallback_2c=%s",
        reason, Describe(r.receiver, a), r.snapshotClass, Describe(r.mode, b), Describe(r.field90, c),
        Describe(r.field9c, d), Describe(r.control14, e), Describe(r.storageBegin, f),
        Describe(r.storageEnd, g), Describe(r.fallback2c, h));
    for (const auto& check : slot.rechecks) {
        if (!slot.recheckAttempted) break;
        if (std::strcmp(check.name, "unavailable") == 0) continue;
        Logger::Line("[SLOT_RECHECK] reason=%s field=%s before=%s after=%s same=%u before_readable=%u after_readable=%u",
            reason, check.name, Describe(check.before, a), Describe(check.after, b), check.same ? 1u : 0u,
            check.before.readable ? 1u : 0u, check.after.readable ? 1u : 0u);
    }
    if (slot.localPointerForFallbackCheck.readable)
        Logger::Line("[SLOT_LOCAL_NULL_CHECK] reason=%s mode=%s local_pointer=%s predicate_only=1 fallback_calls=0 value_metadata_reads=%u",
            reason, Describe(r.mode, a), Describe(slot.localPointerForFallbackCheck, b),
            slot.valueVtable.readable ? 1u : 0u);
    Logger::Line("[SLOT_055F_PASSIVE] reason=%s receiver=%s snapshot_class=%s storage_receiver=%s branch=%s index=0x055f count=%s storage_begin=%s storage_end=%s bounds_valid=%u record=%s flags=%s stored_value_pointer=%s stored_pointer_field_read=%u safe_read=%u slot_consistent=%u status=%s global_ownership=UNRESOLVED",
        reason, Describe(r.receiver, a), r.snapshotClass, Describe(slot.storageReceiver, b), slot.branch,
        Describe(slot.count, c), Describe(slot.begin, d), Describe(slot.end, e),
        slot.boundsValid ? 1u : 0u, Describe(slot.record, f), Describe(slot.flags, g),
        Describe(slot.storedPointer, h), slot.storedPointer.readable ? 1u : 0u,
        slot.safeRead ? 1u : 0u, slot.consistent ? 1u : 0u, slot.reason);
    Logger::Line("[SLOT_055F_TYPE] reason=%s value=%s value_vtable=%s matches_REF_NAMESPACE=%u known_type_match=%s reference_candidate=%s reference_first_word=%s camera_identity=UNRESOLVED",
        reason, Describe(slot.storedPointer, a), Describe(slot.valueVtable, b),
        slot.valueMatchesReferenceVtable ? 1u : 0u,
        slot.valueMatchesReferenceVtable ? "REF_NAMESPACE" : "UNRESOLVED", Describe(slot.referenceCandidate, c),
        Describe(slot.referenceFirstWord, d));
}
void LogSnapshot(const RuntimeProbe::Image& image, const RuntimeProbe::Snapshot& s,
                 const char* reason) noexcept {
    char a[32]{}, b[32]{}, c[32]{}, d[32]{}, e[32]{}, f[32]{};
    Logger::Line("[SNAPSHOT] reason=%s main_base=0x%08lX module_size=0x%08lX identity=VERIFIED observer_thread_only=1",
        reason, static_cast<unsigned long>(image.base), image.size);
    Logger::Line("[HOST] address=0x%08lX context=%s enabled=%s context_vtable=%s get_value_entry=%s",
        static_cast<unsigned long>(s.hostAddress), Describe(s.context, a),
        Describe(s.hostEnabled, b), Describe(s.contextVtable, c), Describe(s.contextGetValueEntry, d));
    Logger::Line("[VM_SAMPLE] depth=%s current_code=%s callback_head=%s candidate_owner=0x%08lX",
        Describe(s.invocationDepth, a), Describe(s.currentCode, b), Describe(s.callbackHead, c),
        static_cast<unsigned long>(s.candidateOwner));
    Logger::Line("[BINDING_CANDIDATE] confidence=LIKELY binding=%s vtable=%s matches_CODE=%u bound_scope_probe=DISABLED",
        Describe(s.ownerBinding, e), Describe(s.bindingVtable, f), s.bindingMatchesCodeVtable ? 1u : 0u);
    LogR0(s, reason);
    Logger::Line("[CAMERA] candidate_camera=UNRESOLVED validation=UNRESOLVED reason=slot_reference_is_not_class_or_current_control_proof");
    Logger::Line("[OUTFIT] current=UNRESOLVED pending=UNRESOLVED normal_choice=UNRESOLVED reason=VM_indices_are_not_native_offsets");
    Logger::Line("[CALLBACK] update_thread=UNRESOLVED frequency=UNRESOLVED camera_in_context=UNRESOLVED hook=NONE touched_game_instructions=0 sample_is_not_callback_execution");
}
bool Changed(const RuntimeProbe::Snapshot& a, const RuntimeProbe::Snapshot& b) noexcept {
    const auto differs = [](RuntimeProbe::Value x, RuntimeProbe::Value y) {
        return x.readable != y.readable || (x.readable && x.raw != y.raw);
    };
    return differs(a.context, b.context) || differs(a.hostEnabled, b.hostEnabled) ||
        differs(a.contextVtable, b.contextVtable) || differs(a.contextGetValueEntry, b.contextGetValueEntry) ||
        differs(a.contextNamespacePayload, b.contextNamespacePayload) ||
        differs(a.invocationDepth, b.invocationDepth) || differs(a.currentCode, b.currentCode) ||
        differs(a.callbackHead, b.callbackHead) || differs(a.ownerBinding, b.ownerBinding) ||
        differs(a.bindingVtable, b.bindingVtable) || differs(a.bindingScopeWord, b.bindingScopeWord) ||
        differs(a.embeddedReference, b.embeddedReference) ||
        differs(a.boundNamespacePayloadCandidate, b.boundNamespacePayloadCandidate) ||
        differs(a.boundNamespacePayloadFirstWord, b.boundNamespacePayloadFirstWord) ||
        differs(a.r0.receiver, b.r0.receiver) || differs(a.r0.mode, b.r0.mode) ||
        differs(a.r0.slot.storedPointer, b.r0.slot.storedPointer) ||
        a.r0.slot.safeRead != b.r0.slot.safeRead;
}
DWORD WINAPI Observe(void*) noexcept {
    // Pin after loader lock is released. This v0.0 supports stop, not dynamic unmapping.
    HMODULE pinnedModule = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN,
        reinterpret_cast<LPCWSTR>(&Observe), &pinnedModule)) {
        InterlockedExchange(&status, 2); return 0;
    }
    RuntimeProbe::Image image;
    const bool accepted = RuntimeProbe::Initialize(image);
    if (!Logger::Open(image.path)) {
        OutputDebugStringW(L"VelvetSwap v0.0: log unavailable; observation disabled.\n");
        InterlockedExchange(&status, 2); return 0;
    }
    SYSTEMTIME loaded{};
    FileTimeToSystemTime(&loadTime, &loaded);
    Logger::Line("[START] VelvetSwap v0.0 x86 observation_only=1 pid=%lu dll_base=0x%08lX load_utc=%04u-%02u-%02uT%02u:%02u:%02u.%03uZ",
        GetCurrentProcessId(), static_cast<unsigned long>(reinterpret_cast<std::uintptr_t>(dllModule)),
        loaded.wYear, loaded.wMonth, loaded.wDay, loaded.wHour, loaded.wMinute, loaded.wSecond, loaded.wMilliseconds);
    Logger::Line("[IDENTITY] path=%ls main_base=0x%08lX module_size=0x%08lX pe_timestamp=0x%08lX sha256=%s result=%s",
        image.path, static_cast<unsigned long>(image.base), image.size, image.timestamp,
        image.sha256[0] ? image.sha256 : "UNRESOLVED", accepted ? "VERIFIED" : "REJECTED");
    constexpr DWORD dllPathCapacity = 32768;
    wchar_t dllPath[dllPathCapacity]{};
    char dllSha256[65]{};
    const DWORD dllPathLength = GetModuleFileNameW(dllModule, dllPath, dllPathCapacity);
    const bool dllHashKnown = dllPathLength && dllPathLength < dllPathCapacity &&
        RuntimeProbe::HashFile(dllPath, dllSha256);
    Logger::Line("[DLL_IDENTITY] path=%ls sha256=%s hash_source=loaded_module_file",
        dllPathLength ? dllPath : L"UNRESOLVED", dllHashKnown ? dllSha256 : "UNRESOLVED");
    if (!accepted) {
        Logger::Line("[DISABLED] %s; game probes=0 hotkey actions=0", image.rejection);
        Logger::Close(); InterlockedExchange(&status, 2); return 0;
    }
    Logger::Line("[POLICY] game_state_writes=0 game_function_calls=0 hooks=0 polling_ms=50 passive_sample_ms=1000 focus_gate=game_process module_pinned_until_process_exit=1");
    Logger::Line("[PROBE_CONTRACT] revision=r0_recheck_v1 current_CODE_only=1 reference_stride=0x20 record0_receiver_offset=0x18 exact_accessor_gate=1 independent_host_VMACHINE_revalidation=1 field_by_field_rechecks=1 slot_index=0x055f slot_stride=0x10 legacy_CODE_scope_probe=DISABLED");
    auto previous = RuntimeProbe::Capture(image);
    LogSnapshot(image, previous, "startup");
    InterlockedExchange(&status, 1);
    bool held[5]{};
    ULONGLONG lastSample = GetTickCount64();
    ULONGLONG samples = 1;
    ULONGLONG changes = 0;
    ULONGLONG lastHeartbeat = lastSample;
    constexpr const char* labels[5] = {"Campaign", "Pilot", "Nightdress", "SS uniform", "Restore"};
    while (!InterlockedCompareExchange(&stopRequested, 0, 0)) {
        DWORD foregroundPid = 0;
        GetWindowThreadProcessId(GetForegroundWindow(), &foregroundPid);
        const bool focused = foregroundPid == GetCurrentProcessId();
        for (int i = 0; i < 5; ++i) {
            const bool down = (GetAsyncKeyState(VK_F1 + i) & 0x8000) != 0;
            if (focused && down && !held[i]) {
                Logger::Line("[F%d] %s requested - observation only", i + 1, labels[i]);
                char reason[8]{};
                sprintf_s(reason, "F%d", i + 1);
                LogSnapshot(image, RuntimeProbe::Capture(image), reason);
            }
            held[i] = down;
        }
        const auto now = GetTickCount64();
        if (now - lastSample >= 1000) {
            const auto sample = RuntimeProbe::Capture(image);
            ++samples;
            LogR0(sample, "passive_sample");
            if (Changed(previous, sample)) {
                ++changes;
                Logger::Line("[PASSIVE_CHANGE] sample=%llu changes=%llu correlation=UNRESOLVED not_OnLoad_or_HandlePlayerSwitches_event", samples, changes);
                LogSnapshot(image, sample, "passive_change");
            }
            previous = sample;
            lastSample = now;
            if (now - lastHeartbeat >= 60000) {
                Logger::Line("[PASSIVE_HEARTBEAT] samples=%llu changed_samples=%llu sampled_stability_only=1 camera_identity=UNRESOLVED", samples, changes);
                lastHeartbeat = now;
            }
        }
        Sleep(50);
    }
    Logger::Line("[STOP] observation stopped passive_samples=%llu changed_samples=%llu game_state_writes=0", samples, changes);
    Logger::Close();
    InterlockedExchange(&status, 3);
    return 0;
}
}
extern "C" DWORD WINAPI VelvetSwapStop(void*) noexcept {
    InterlockedExchange(&stopRequested, 1);
    return 0;
}
extern "C" DWORD WINAPI VelvetSwapStatus(void*) noexcept {
    return static_cast<DWORD>(InterlockedCompareExchange(&status, 0, 0));
}
BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        dllModule = module;
        GetSystemTimeAsFileTime(&loadTime);
        DisableThreadLibraryCalls(module);
        // No waits, logger, probing, hashing or VM operations under loader lock.
        const HANDLE worker = CreateThread(nullptr, 0, &Observe, nullptr, 0, nullptr);
        if (!worker) return FALSE;
        CloseHandle(worker);
    } else if (reason == DLL_PROCESS_DETACH) {
        InterlockedExchange(&stopRequested, 1);
        // Never wait for a worker from DllMain.
    }
    return TRUE;
}
