#include "converter.h"
#include "SigScan.h"
#include <Windows.h>

PC::CONVERTER::LONG_TO_INT_ADDRESS_t PC::CONVERTER::LONG_TO_INT_ADDRESS = SignatureScan<PC::CONVERTER::LONG_TO_INT_ADDRESS_t>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x85\xC9\x75\x08\x33\xC0\x48\x83\xC4\x20\x5B\xC3\xE8\x65", "xxxxxxxxxxxxxxxxxxxxxxxx");
PC::CONVERTER::INT_TO_LONG_ADDRESS_t PC::CONVERTER::INT_TO_LONG_ADDRESS = SignatureScan<PC::CONVERTER::INT_TO_LONG_ADDRESS_t>("\x40\x53\x48\x83\xEC\x20\x8B\xD9\xE8\x00\x00\x00\x00\x0F\xBA\xF3\x1F\x48\x8D\x15\x00\x00\x00\x00", "xxxxxxxxx????xxxxxxx????");