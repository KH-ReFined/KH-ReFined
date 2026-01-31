#include "sequence.h"
#include "SigScan.h"

YI::SEQUENCE::CreateNew_t YI::SEQUENCE::CreateNew = ResolveFunctionFromCall<YI::SEQUENCE::CreateNew_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x167);

YI::SEQUENCE::Init_t YI::SEQUENCE::Init = ResolveFunctionFromCall<YI::SEQUENCE::Init_t>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0xD9);
YI::SEQUENCE::SetNumber_t YI::SEQUENCE::SetNumber = ResolveFunctionFromCall<YI::SEQUENCE::SetNumber_t>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0xE6);
YI::SEQUENCE::GetParamCr_t YI::SEQUENCE::GetParamCr = ResolveFunctionFromCall<YI::SEQUENCE::GetParamCr_t>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0x104);

YI::SEQUENCE::GetParamEx_t YI::SEQUENCE::GetParamEx = reinterpret_cast<YI::SEQUENCE::GetParamEx_t>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamCr) + 0x20);
YI::SEQUENCE::SetNumberForce_t YI::SEQUENCE::SetNumberForce = reinterpret_cast<YI::SEQUENCE::SetNumberForce_t>(reinterpret_cast<char*>(YI::SEQUENCE::SetNumber) + 0x30);
