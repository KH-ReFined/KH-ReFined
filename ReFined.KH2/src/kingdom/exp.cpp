#include "exp.h"
#include "SigScan.h"

dk::Exp::getSeqNumber_t dk::Exp::getSeqNumber = SignatureScan<dk::Exp::getSeqNumber_t>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\x05", "xxxxxxxxxxxxxxxxxx");