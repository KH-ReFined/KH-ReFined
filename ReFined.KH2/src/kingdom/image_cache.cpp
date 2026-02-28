#include "image_cache.h"

YI::IMAGE_CACHE::Load_t YI::IMAGE_CACHE::Load = SignatureScan<YI::IMAGE_CACHE::Load_t>("\x48\x89\x5C\x24\x18\x48\x89\x6C\x24\x20\x41\x56\x48\x83\xEC\x20\x44\x8B\x89\xF4\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx");