#include "axa.h"

Axa::ResourceEntry::ReadDataLocal_t Axa::ResourceEntry::ReadDataLocal = SignatureScan<Axa::ResourceEntry::ReadDataLocal_t>("\x4C\x8B\x91\x70\x06\x00\x00\x4D\x8B\xD9\x4D\x85\xD2", "xxxxxxxxxxxxx");