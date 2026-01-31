#include "region.h"

YS::REGION::Get_t YS::REGION::Get = ResolveFunctionFromCall<YS::REGION::Get_t>("\x40\x57\x48\x83\xEC\x20\x0F\xB6\x41\x48", "xxxxxxxxxx", 0x2E);

uint64_t* YS::REGION::pint_region = ResolveRelativeAddress<uint64_t*>("\x40\x57\x48\x83\xEC\x20\x0F\xB6\x41\x48", "xxxxxxxxxx", 0x44);
uint64_t* YS::REGION::pint_region_default = ResolveRelativeAddress<uint64_t*>("\x40\x57\x48\x83\xEC\x20\x0F\xB6\x41\x48", "xxxxxxxxxx", 0x44) - 0x08;