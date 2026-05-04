#include "cmenuhelp.h"

void (*Tz::CMenuHelp::Create)(char*, bool) = SignatureScan<void(*)(char*, bool)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x50\x48\x8B\xD9\x0F\xB6\xFA\x48\x8B\x0D", "xxxxxxxxxxxxxxxxxxx");