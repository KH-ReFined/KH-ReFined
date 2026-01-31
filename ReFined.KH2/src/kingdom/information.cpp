#include "information.h"
#include "SigScan.h"
#include <Windows.h>

dk::INFORMATION::openInformationWindow_t dk::INFORMATION::openInformationWindow = SignatureScan<dk::INFORMATION::openInformationWindow_t>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\xD3", "xxxxxxxxxxxx????x????xxx????xxx");
dk::INFORMATION::openGetMunny_t dk::INFORMATION::openGetMunny = ResolveFunctionFromCall<dk::INFORMATION::openGetMunny_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x30\x41\x0F\xB6\x58", "xxxxxxxxxxxxxxxxxxxxxxxx", 0xB1);