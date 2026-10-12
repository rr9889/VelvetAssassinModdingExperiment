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
void LogBindingScope(const RuntimeProbe::Snapshot& s, const char* reason) noexcept {
    char a[32]{}, b[32]{}, c[32]{}, d[32]{}, e[32]{};
    Logger::Line("[BINDING_SCOPE] reason=%s code=%s ref_object=%s ref_vtable=%s ref_vtable_match=%u namespace_payload_candidate=%s payload_nonnull=%u payload_readable=%u payload_first_word=%s payload_identity=UNRESOLVED",
        reason, Describe(s.ownerBinding, a), Describe(s.embeddedReference, b),
        Describe(s.bindingScopeWord, c), s.bindingScopeMatchesReferenceVtable ? 1u : 0u,
        Describe(s.boundNamespacePayloadCandidate, d),
        s.boundNamespacePayloadCandidate.readable && s.boundNamespacePayloadCandidate.raw ? 1u : 0u,
        s.boundNamespacePayloadFirstWord.readable ? 1u : 0u,
        Describe(s.boundNamespacePayloadFirstWord, e));
}
void LogSnapshot(const RuntimeProbe::Image& image, const RuntimeProbe::Snapshot& s,
                 const char* reason) noexcept {
    char a[32]{}, b[32]{}, c[32]{}, d[32]{}, e[32]{}, f[32]{}, g[32]{}, h[32]{};
    Logger::Line("[SNAPSHOT] reason=%s main_base=0x%08lX module_size=0x%08lX identity=VERIFIED observer_thread_only=1",
        reason, static_cast<unsigned long>(image.base), image.size);
    Logger::Line("[HOST] address=0x%08lX context=%s enabled=%s context_vtable=%s get_value_entry=%s",
        static_cast<unsigned long>(s.hostAddress), Describe(s.context, a),
        Describe(s.hostEnabled, b), Describe(s.contextVtable, c), Describe(s.contextGetValueEntry, d));
    Logger::Line("[VM_SAMPLE] depth=%s current_code=%s callback_head=%s candidate_owner=0x%08lX",
        Describe(s.invocationDepth, a), Describe(s.currentCode, b), Describe(s.callbackHead, c),
        static_cast<unsigned long>(s.candidateOwner));
    Logger::Line("[BINDING_CANDIDATE] confidence=LIKELY binding=%s vtable=%s matches_CODE=%u scope_word=%s scope_semantics=UNRESOLVED",
        Describe(s.ownerBinding, e), Describe(s.bindingVtable, f), s.bindingMatchesCodeVtable ? 1u : 0u,
        Describe(s.bindingScopeWord, g));
    LogBindingScope(s, reason);
    Logger::Line("[REF_METADATA] context_matches_REF_NAMESPACE=%u payload=%s camera_identity=UNRESOLVED",
        s.contextMatchesReferenceVtable ? 1u : 0u, Describe(s.contextNamespacePayload, h));
    Logger::Line("[CAMERA] slot_0x055f_raw=UNRESOLVED candidate_camera=UNRESOLVED validation=UNRESOLVED reason=native_slot_storage_not_verified");
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
        differs(a.boundNamespacePayloadFirstWord, b.boundNamespacePayloadFirstWord);
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
    if (!accepted) {
        Logger::Line("[DISABLED] %s; game probes=0 hotkey actions=0", image.rejection);
        Logger::Close(); InterlockedExchange(&status, 2); return 0;
    }
    Logger::Line("[POLICY] game_state_writes=0 game_function_calls=0 hooks=0 polling_ms=50 passive_sample_ms=1000 focus_gate=game_process module_pinned_until_process_exit=1");
    Logger::Line("[PROBE_CONTRACT] revision=bound_namespace_payload_v1 embedded_ref_offset=0x24 exact_REF_NAMESPACE_vtable_gate=1 payload_offset=0x18 passive_scope_samples=1");
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
            LogBindingScope(sample, "passive_sample");
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
