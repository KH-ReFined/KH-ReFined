#include "weapon_mset.h"

YS::WEAPON_MSET::GetFilename_t YS::WEAPON_MSET::GetFilename = SignatureScan<YS::WEAPON_MSET::GetFilename_t>("\x48\x83\xEC\x28\x48\x63\xC2\x4C\x63\xC9\x4A\x8D\x14\x48\x48\xC1", "xxxxxxxxxxxxxxxx");