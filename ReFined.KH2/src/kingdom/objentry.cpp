#include "objentry.h"

YS::OBJENTRY::GetFriendPriority_t YS::OBJENTRY::GetFriendPriority = ResolveFunctionFromCall<YS::OBJENTRY::GetFriendPriority_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x111);
YS::OBJENTRY::Get_t YS::OBJENTRY::Get = ResolveFunctionFromCall<YS::OBJENTRY::Get_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x1E2);
YS::OBJENTRY::ReadRequestWeapon_t YS::OBJENTRY::ReadRequestWeapon = ResolveFunctionFromCall<YS::OBJENTRY::ReadRequestWeapon_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x169);

YS::OBJENTRY::get_mdlx_t YS::OBJENTRY::get_mdlx = SignatureScan<YS::OBJENTRY::get_mdlx_t>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x85\xD2\x48\x8D\x79\x08", "xxxxxxxxxxxxxxxxx");
YS::OBJENTRY::get_apdx_t YS::OBJENTRY::get_apdx = SignatureScan<YS::OBJENTRY::get_apdx_t>("\x40\x57\x48\x83\xEC\x20\x0F\xB6\x41\x48", "xxxxxxxxxx");
YS::OBJENTRY::get_mset_t YS::OBJENTRY::get_mset = SignatureScan<YS::OBJENTRY::get_mset_t>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x20\x4D\x85\xC0\x48\x8D\x3D", "xxxxxxxxxxxxxxxxxxxxxxxxxx");