#include "mission_count.h"

YS::MISSION_COUNT::destroy_t YS::MISSION_COUNT::destroy = ResolveRelativeAddress<YS::MISSION_COUNT::destroy_t>(reinterpret_cast<char*>(YS::LIMIT::destroy), 0xF9);