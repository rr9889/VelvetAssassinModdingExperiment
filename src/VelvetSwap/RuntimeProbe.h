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
struct FieldRecheck {
    const char* name = "unavailable";
    Value before;
    Value after;
    bool same = false; // False includes unreadable; never equate two failed reads.
};
inline constexpr std::size_t AcquisitionRecheckCount = 10;
inline constexpr std::size_t StorageRecheckCount = 10;
struct SlotObservation {
    Value storageReceiver;
    Value begin;
    Value end;
    Value count;
    Value record;
    Value flags;
    Value localPointerForFallbackCheck; // Modes 4/6 local NULL predicate, never an unwrap result.
    Value storedPointer;
    Value valueVtable;
    Value referenceCandidate;
    Value referenceFirstWord;
    const char* branch = "UNRESOLVED";
    const char* reason = "receiver_unavailable";
    bool boundsValid = false;
    bool consistent = false;
    bool safeRead = false; // Validated direct-return branch, not merely a readable address.
    bool valueMatchesReferenceVtable = false;
    FieldRecheck rechecks[StorageRecheckCount]{};
    bool recheckAttempted = false;
};
struct R0Observation {
    Value codeVtable;
    Value tableBegin;
    Value tableEnd;
    Value tableCount;
    Value record0;
    Value receiver;
    Value vtable;
    Value virtual98;
    Value mode;
    Value field90;
    Value field9c;
    Value control14;
    Value storageBegin;
    Value storageEnd;
    Value fallback2c;
    SlotObservation slot;
    const char* reason = "current_code_unavailable";
    bool codeMatches = false;
    bool tableBoundsValid = false;
    bool receiverReadable = false;
    bool comparisonKnown = false;
    bool equalsHostContext = false;
    bool matchesAccessor = false;
    bool consistent = false; // Rechecked fields; still not an atomic VM snapshot.
    FieldRecheck rechecks[AcquisitionRecheckCount]{};
    const char* snapshotClass = "UNAVAILABLE";
    bool recheckAttempted = false;
    bool afterTableBoundsValid = false;
    bool receiverRevalidated = false;
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
    Value bindingScopeWord; // First word/vtable at CODE+0x24; never the payload.
    Value embeddedReference; // Computed address CODE+0x24; readable marks address availability.
    Value boundNamespacePayloadCandidate;
    Value boundNamespacePayloadFirstWord;
    std::uintptr_t candidateOwner = 0;
    bool bindingMatchesCodeVtable = false;
    bool bindingScopeMatchesReferenceVtable = false;
    bool contextMatchesReferenceVtable = false;
    R0Observation r0;
    // No camera/outfit fields: raw slot observation does not prove camera identity.
};
bool Readable(const void* address, std::size_t bytes) noexcept;
bool SafeRead(const void* source, void* destination, std::size_t bytes) noexcept;
bool InModule(const Image& image, std::uintptr_t address, std::size_t bytes) noexcept;
bool Rebase(const Image& image, std::uintptr_t preferredAddress,
            std::size_t bytes, std::uintptr_t& address) noexcept;
bool HashFile(const wchar_t* path, char (&hex)[65]) noexcept;
bool Initialize(Image& image) noexcept;
// Historical scope reads are disabled for the DLL; true is used only by old owned fixtures.
Snapshot Capture(const Image& image, bool historicalScopeFixture = false) noexcept;
#ifdef VELVETSWAP_OWNED_FIXTURE
// Only compiled into the owned-memory test executable, never into the observation DLL.
extern void (*OwnedFixtureBeforeRecheck)() noexcept;
#endif
}
