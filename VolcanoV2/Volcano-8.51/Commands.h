#pragma once
#include "framework.h"
#include "Inventory.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>

class FFrame;
static void (*FrameStep)(FFrame*, SDK::UObject*, void* const) = decltype(FrameStep)(GetOffsetBRUH(0x1e83f60));
static void (*FrameStepExplicit)(FFrame*, void* const, SDK::UField*) = decltype(FrameStepExplicit)(GetOffsetBRUH(0x1e83f90));

static void StepCompiledIn(FFrame* Stack, void* Result)
{
	auto Base = (uint8_t*)Stack;
	uint8_t* Code = *(uint8_t**)(Base + 0x20);
	if (Code)
	{
		SDK::UObject* Obj = *(SDK::UObject**)(Base + 0x18);
		FrameStep(Stack, Obj, Result);
	}
	else
	{
		void** Chain = (void**)(Base + 0x68);
		void* Property = *Chain;
		if (Property)
		{
			*Chain = *(void**)((uint8_t*)Property + 0x28);
			FrameStepExplicit(Stack, Result, (SDK::UField*)Property);
		}
	}
}

static std::vector<std::string> SplitArgs(const std::string& Input)
{
	std::vector<std::string> Out;
	size_t pos = 0, last = 0;
	while ((pos = Input.find(' ', last)) != std::string::npos)
	{
		if (pos > last)
			Out.push_back(Input.substr(last, pos - last));
		last = pos + 1;
	}
	if (last < Input.size())
		Out.push_back(Input.substr(last));
	return Out;
}

void RunCommand(AFortPlayerControllerAthena* PC, const std::string& Full)
{
	auto Args = SplitArgs(Full);
	if (Args.empty())
		return;

	std::string Cmd = Args[0];
	std::transform(Cmd.begin(), Cmd.end(), Cmd.begin(), [](unsigned char c) { return (char)tolower(c); });

	auto Pawn = (AFortPlayerPawnAthena*)PC->Pawn;

	if (Cmd == "sethealth" && Args.size() > 1 && Pawn)
	{
		Pawn->SetHealth((float)atoi(Args[1].c_str()));
		LOG_("cmd: sethealth {}", Args[1]);
	}
	else if (Cmd == "setshield" && Args.size() > 1 && Pawn)
	{
		Pawn->SetShield((float)atoi(Args[1].c_str()));
		LOG_("cmd: setshield {}", Args[1]);
	}
	else if (Cmd == "giveitem" && Args.size() > 1)
	{
		int Count = Args.size() > 2 ? atoi(Args[2].c_str()) : 1;
		if (auto Def = StaticLoadObject<UFortItemDefinition>(Args[1]))
		{
			Inventory::AddItem(PC, Def, Count);
			LOG_("cmd: giveitem {} x{}", Args[1], Count);
		}
	}
	else if (Cmd == "tp" && Args.size() > 3 && Pawn)
	{
		FVector Loc{ (float)atof(Args[1].c_str()), (float)atof(Args[2].c_str()), (float)atof(Args[3].c_str()) };
		Pawn->K2_TeleportTo(Loc, Pawn->K2_GetActorRotation());
		LOG_("cmd: tp {} {} {}", Args[1], Args[2], Args[3]);
	}
	else if (Cmd == "spawnactor" && Args.size() > 1 && Pawn)
	{
		if (auto Class = StaticLoadObject<UClass>(Args[1]))
		{
			FVector Loc = Pawn->K2_GetActorLocation();
			Loc.X += 500;
			SpawnActor<AActor>(Class, Loc);
			LOG_("cmd: spawnactor {}", Args[1]);
		}
	}
	else if (Cmd == "infiniteammo")
	{
		Globals::bInfiniteAmmo = !Globals::bInfiniteAmmo;
		LOG_("cmd: infiniteammo {}", Globals::bInfiniteAmmo);
	}
	else if (Cmd == "infinitemats")
	{
		Globals::bInfiniteMats = !Globals::bInfiniteMats;
		LOG_("cmd: infinitemats {}", Globals::bInfiniteMats);
	}
	else if (Cmd == "help")
	{
		LOG_("============ Commands ============ \n\n cheat infinitemats \n cheat infiniteammo \n cheat sethealth \n cheat spawnactor \n giveitem id n \n cheat tp x y z \n cheat setshield")
	}
	else
	{
		LOG_("cmd: unknown '{}'", Cmd);
	}
}

void ServerCheatHook(SDK::UObject* Context, void* Stack, void* Result)
{
	FString Msg{};
	StepCompiledIn((FFrame*)Stack, &Msg);

	auto PC = (AFortPlayerControllerAthena*)Context;
	if (PC)
		RunCommand(PC, Msg.ToString());
}

void InitCommands()
{
	auto Fn = UObject::FindObjectFast<UFunction>("ServerCheat");
	if (!Fn)
	{
		LOG_("ServerCheat UFunction not found, commands disabled");
		return;
	}

	auto ExecPtr = (void**)(__int64(Fn) + 0xC0);
	LOG_("ServerCheat Func @0xC0 = 0x{:x}", __int64(*ExecPtr) - __int64(GetModuleHandleW(0)));
	*ExecPtr = (void*)ServerCheatHook;
	LOG_("commands hooked via ServerCheat");
}
