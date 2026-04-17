#include "save_indicator.h"

char* YS::SAVE_INDICATOR::image = nullptr;
char* YS::SAVE_INDICATOR::instance = nullptr;
char* YS::SAVE_INDICATOR::layoutFile = nullptr;

char* YS::SAVE_INDICATOR::VTABLE_CLASS = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x154);
char* YS::SAVE_INDICATOR::VTABLE_SPRITE = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x80);

void YS::SAVE_INDICATOR::init()
{
 
}

void YS::SAVE_INDICATOR::create(int offset16x9)
{

}