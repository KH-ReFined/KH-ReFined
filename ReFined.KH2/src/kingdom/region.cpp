#include "region.h"

YS::REGION::Get_t YS::REGION::Get = ResolveFunctionFromCall<YS::REGION::Get_t>("\x40\x53\x48\x83\xEC\x30\x48\x83\x79\x40\x00\x48\x8B\xD9\x75\x28", "xxxxxxxxxxxxxxxx", 0x3F);

char** YS::REGION::Region = ResolveRelativeAddress<char**>("\x83\xF9\x07\x0F\x87\xDC\x00\x00\x00\x48\x63\xC1", "xxxxxxxxxxxx", 0x37);
char** YS::REGION::DefaultRegion = ResolveRelativeAddress<char**>("\x83\xF9\x07\x0F\x87\xDC\x00\x00\x00\x48\x63\xC1", "xxxxxxxxxxxx", 0x29);