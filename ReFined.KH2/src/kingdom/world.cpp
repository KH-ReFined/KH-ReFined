#include "world.h"
#include "SigScan.h"
#include <Windows.h>

WORLD::GetName_t WORLD::GetName = ResolveFunctionFromCall<WORLD::GetName_t>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xD1\x83\xEA\x64\x74", "xxxxxxxxxxxxxxxx", 0x1B);