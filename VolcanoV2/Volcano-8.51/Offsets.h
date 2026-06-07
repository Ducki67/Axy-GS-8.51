// ty Ploosh's offset dumper but plz add s21+ support :pray:
// Fortnite version: 8.51
#include <stdint.h>
#include <array>
namespace SDK {
    class UNetDriver;
    class UObject;
    class UClass;
    class UField;
    class FField;
    class UEngine;
    class FName;
    struct FURL;
    class UReplicationDriver;
    class UWorld;
    class AOnlineBeaconHost;
};
namespace UC {
    class FString;
}
class FFrame;

namespace VolcanoV2 {
    inline auto ImageBase = *(uint64_t*)(__readgsqword(0x60) + 0x10);
    namespace Offsets {
        inline uint64_t Step = 0x1e83f60;
        inline uint64_t StepExplicitProperty = 0x1e83f90;
        inline uint64_t CreateNetDriver = 0x2fbed30;
        inline uint64_t InitListen = 0x634c10;
        inline uint64_t SetWorld = 0x2d38590;
        inline uint64_t GetNetMode = 0x30100a0;
        inline uint64_t TickFlush = 0x2d39300;
        inline uint64_t ServerReplicateActors = 0x95edb0;
        inline uint64_t GetMaxTickRate = 0x2fc1700;
        inline uint64_t DispatchRequest = 0xcf2e80;
        inline uint64_t Realloc = 0x1c08880;
        inline uint64_t StaticFindObject = 0x1e825f0;
        inline uint64_t StaticLoadObject = 0x1e838d0;
        inline uint64_t GIsClient = 0x5940a13;
        inline uint64_t GameSessionPatch = 0xff343c;
        inline uint64_t EncryptionPatch = 0x3012aea;
        inline std::array<uint64_t, 4> NullFuncs = { 0x2815170, 0x12e7410, 0x2c03d20 };
        inline std::array<uint64_t, 0> RetTrueFuncs = {  };
    };

    namespace Vfts {
    };

    namespace Funcs {
        inline auto Step = (void (*)(FFrame*, SDK::UObject*, void* const)) (ImageBase + Offsets::Step);
        inline auto StepExplicitProperty = (void (*)(FFrame*, void* const, SDK::UField*)) (ImageBase + Offsets::StepExplicitProperty);
        inline auto CreateNetDriver = (SDK::UNetDriver* (*)(SDK::UEngine*, SDK::UWorld*, SDK::FName)) (ImageBase + Offsets::CreateNetDriver);
        inline auto InitListen = (bool (*)(SDK::UNetDriver*, SDK::UWorld*, SDK::FURL&, bool, UC::FString&)) (ImageBase + Offsets::InitListen);
        inline auto SetWorld = (void (*)(SDK::UNetDriver*, SDK::UWorld*)) (ImageBase + Offsets::SetWorld);
        inline auto ServerReplicateActors = (void (*)(SDK::UReplicationDriver*, float)) (ImageBase + Offsets::ServerReplicateActors);
        inline auto Realloc = (void* (*)(void*, __int64, unsigned int)) (ImageBase + Offsets::Realloc);
        inline auto StaticFindObject = (SDK::UObject* (*)(SDK::UClass*, SDK::UObject*, const wchar_t*, bool)) (ImageBase + Offsets::StaticFindObject);
        inline auto StaticLoadObject = (SDK::UObject* (*)(SDK::UClass*, SDK::UObject*, const wchar_t*, const wchar_t*, uint32_t, SDK::UObject*, bool)) (ImageBase + Offsets::StaticLoadObject);
    };
};
using namespace VolcanoV2;
using namespace VolcanoV2::Offsets;
using namespace VolcanoV2::Vfts;
using namespace VolcanoV2::Funcs;
