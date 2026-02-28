#include "sequence.h"

YI::SEQUENCE::SEQUENCE_t YI::SEQUENCE::_SEQUENCE = ResolveFunctionFromCall<YI::SEQUENCE::SEQUENCE_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x167);
YI::SEQUENCE::Dispose_t YI::SEQUENCE::Dispose = ResolveFunctionFromCall<YI::SEQUENCE::Dispose_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x8B\xEA\x48\x8D\xB9\x90\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x3A);

YI::SEQUENCE::Init_t YI::SEQUENCE::Init = ResolveFunctionFromCall<YI::SEQUENCE::Init_t>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0xD9);
YI::SEQUENCE::SetNumber_t YI::SEQUENCE::SetNumber = ResolveFunctionFromCall<YI::SEQUENCE::SetNumber_t>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0xE6);
YI::SEQUENCE::GetParamCr_t YI::SEQUENCE::GetParamCr = ResolveFunctionFromCall<YI::SEQUENCE::GetParamCr_t>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0x104);

YI::SEQUENCE::Control_t YI::SEQUENCE::Control = SignatureScan<YI::SEQUENCE::Control_t>("\x40\x53\x48\x83\xEC\x20\xF3\x0F\x10\x0D\x00\x00\x00\x00\x48\x8B\xD9\xF3\x0F\x59\x49\x68\x0F\x28\xC1\xF3\x0F\x58\x81\x44\x01\x00\x00\xF3\x0F\x11\x81\x44\x01\x00\x00\x8B\x89\x40\x01\x00\x00\x83\xE9\x01\x0F\x84\xA5\x00\x00\x00\x83\xF9\x01\x0F\x85\xD1", "xxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
YI::SEQUENCE::Draw_t YI::SEQUENCE::Draw = SignatureScan<YI::SEQUENCE::Draw_t>("\x4C\x8B\xDC\x53\x48\x81\xEC\xD0\x00\x00\x00\x83\xB9\x40\x01\x00\x00\x00\x48\x8B\xD9", "xxxxxxxxxxxxxxxxxxxxx");

YI::SEQUENCE::GetParamEx_t YI::SEQUENCE::GetParamEx = reinterpret_cast<YI::SEQUENCE::GetParamEx_t>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamCr) + 0x20);
YI::SEQUENCE::SetNumberForce_t YI::SEQUENCE::SetNumberForce = reinterpret_cast<YI::SEQUENCE::SetNumberForce_t>(reinterpret_cast<char*>(YI::SEQUENCE::SetNumber) + 0x30);

YI::SEQUENCE::GetParamSize_t YI::SEQUENCE::GetParamSize = reinterpret_cast<YI::SEQUENCE::GetParamSize_t>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamEx) + 0x20);
YI::SEQUENCE::GetParamX_t YI::SEQUENCE::GetParamX = reinterpret_cast<YI::SEQUENCE::GetParamX_t>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamEx) + 0x40);
YI::SEQUENCE::GetParamY_t YI::SEQUENCE::GetParamY = reinterpret_cast<YI::SEQUENCE::GetParamY_t>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamEx) + 0x60);

YI::SEQUENCE::GetActiveX_t YI::SEQUENCE::GetActiveX = reinterpret_cast<YI::SEQUENCE::GetActiveX_t>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamEx) - 0x250);
YI::SEQUENCE::GetActiveY_t YI::SEQUENCE::GetActiveY = reinterpret_cast<YI::SEQUENCE::GetActiveY_t>(reinterpret_cast<char*>(YI::SEQUENCE::GetActiveX) + 0x80);
YI::SEQUENCE::SetScaleX_t YI::SEQUENCE::SetScaleX = SignatureScan<YI::SEQUENCE::SetScaleX_t>("\xF3\x0F\x10\x81\x78\x01\x00\x00\xF3\x0F\x10\x15\x00\x00\x00\x00\x0F\x2E\xC2\xF3\x0F\x11\x89\x7C\x01\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxx");

