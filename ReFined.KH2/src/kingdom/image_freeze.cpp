#include "image_freeze.h"

YI::IMAGE_FREEZE::GetImage_t YI::IMAGE_FREEZE::GetImage = SignatureScan<YI::IMAGE_FREEZE::GetImage_t>("\x48\x63\xC2\x48\xC1\xE0\x08\x48\x03\x41\x28\xC3", "xxxxxxxxxxxx");