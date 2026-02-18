#include "task.h"

TASK::sleep_t TASK::sleep = ResolveFunctionFromCall<TASK::sleep_t>("\x85\xC9\x0F\x84\x93\x01\x00\x00\x41\x56\x48\x83\xEC\x70\x48\x8B", "xxxxxxxxxxxxxxxx", 0x169);